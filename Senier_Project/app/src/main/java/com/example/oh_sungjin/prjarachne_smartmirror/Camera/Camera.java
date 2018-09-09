package com.example.oh_sungjin.prjarachne_smartmirror.Camera;
import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.CaptureResult;
import android.hardware.camera2.TotalCaptureResult;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.ImageReader;
import android.os.Handler;
import android.util.Log;
import android.util.Size;

import java.util.Collections;

import static android.content.Context.CAMERA_SERVICE;

/**
 * Helper class to deal with methods to deal with images from the camera.
 */
public class Camera {
    private static final String TAG = Camera.class.getSimpleName();

    private static final int IMAGE_WIDTH = 640;
    private static final int IMAGE_HEIGHT = 360;
    private static final int MAX_IMAGES = 3;
    //사진 사이즈 및 수

    private CameraDevice mCameraDevice;
    //카메라 장치

    private CameraCaptureSession mCaptureSession;
    //카메라 캡쳐

    private ImageReader mImageReader;

    // Lazy-loaded singleton, so only one instance of the camera is created.
    private Camera() {
    }

    private static class InstanceHolder {
        private static Camera mCamera = new Camera();
    }

    public static Camera getInstance() {
        return InstanceHolder.mCamera;
    }

    public void initializeCamera(Context context,
                                 Handler backgroundHandler,
                                 ImageReader.OnImageAvailableListener imageAvailableListener) {

        CameraManager manager = (CameraManager) context.getSystemService(CAMERA_SERVICE);
        String[] camIds = {};
        try {
            camIds = manager.getCameraIdList();
        } catch (CameraAccessException e) {
            Log.e(TAG, "Cam access exception getting IDs", e);
        }
        if (camIds.length < 1) {
            Log.e(TAG, "No cameras found");
            return;
        }
        String id = camIds[0];
        //카메라 장치 초기화

        mImageReader = ImageReader.newInstance(IMAGE_WIDTH, IMAGE_HEIGHT,
                ImageFormat.JPEG, MAX_IMAGES);
        mImageReader.setOnImageAvailableListener(
                imageAvailableListener, backgroundHandler);
        //이미지 초기화

        try {
            manager.openCamera(id, mStateCallback, backgroundHandler);
        } catch (CameraAccessException cae) {
            Log.d(TAG, "Camera access exception", cae);
        }
        //카메라 오픈
    }

    //카메라 상태 변화에 따른 콜백
    private final CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {
        @Override
        public void onOpened(CameraDevice cameraDevice) {
            mCameraDevice = cameraDevice;
        }

        @Override
        public void onDisconnected(CameraDevice cameraDevice) {
            cameraDevice.close();
        }

        @Override
        public void onError(CameraDevice cameraDevice, int i) {
            cameraDevice.close();
        }

        @Override
        public void onClosed(CameraDevice cameraDevice) {
            mCameraDevice = null;
        }
    };

    //사진 캡쳐
    public void takePicture() {
        if (mCameraDevice == null) {
            return;
        }
        //장치 초기화 실패

        //사진 캡쳐
        try {
            mCameraDevice.createCaptureSession(
                    Collections.singletonList(mImageReader.getSurface()),
                    mSessionCallback,
                    null);
        } catch (CameraAccessException cae) {
            Log.e(TAG, "access exception while preparing pic", cae);
        }
    }

    //세션 상태 변화에 따른 콜백
    private CameraCaptureSession.StateCallback mSessionCallback =
            new CameraCaptureSession.StateCallback() {
                @Override
                public void onConfigured(CameraCaptureSession cameraCaptureSession) {
                    //카메라가 이미 종료
                    if (mCameraDevice == null) {
                        return;
                    }

                    //세션 준비 완료, 캡쳐 시작
                    mCaptureSession = cameraCaptureSession;
                    triggerImageCapture();
                }

                @Override
                public void onConfigureFailed(CameraCaptureSession cameraCaptureSession) {
                    Log.e(TAG, "Failed to configure camera");
                }
            };

    //세션이 동작하고 캡쳐 요청이 있을 경우 실행
    private void triggerImageCapture() {
        try {
            final CaptureRequest.Builder captureBuilder =
                    mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE);
            captureBuilder.addTarget(mImageReader.getSurface());
            captureBuilder.set(CaptureRequest.CONTROL_AE_MODE, CaptureRequest.CONTROL_AE_MODE_ON);
            //세션 초기화
            mCaptureSession.capture(captureBuilder.build(), mCaptureCallback, null);
        } catch (CameraAccessException cae) {
            Log.e(TAG, "camera capture exception", cae);
        }
    }

    //캡쳐 세션 이벤트에 따른 콜백
    private final CameraCaptureSession.CaptureCallback mCaptureCallback =
            new CameraCaptureSession.CaptureCallback() {

                @Override
                public void onCaptureProgressed(CameraCaptureSession session,
                                                CaptureRequest request,
                                                CaptureResult partialResult) {
                }

                @Override
                public void onCaptureCompleted(CameraCaptureSession session,
                                               CaptureRequest request,
                                               TotalCaptureResult result) {
                    if (session != null) {
                        session.close();
                        mCaptureSession = null;
                        //캡쳐 완료
                    }
                }
            };


    //카메라 종료
    public void shutDown() {
        if (mCameraDevice != null) {
            mCameraDevice.close();
        }
    }
}