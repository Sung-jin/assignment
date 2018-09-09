package com.example.oh_sungjin.prjarachne_smartmirror;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.Image;
import android.media.ImageReader;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.ImageView;

import com.example.oh_sungjin.prjarachne_smartmirror.Camera.Camera;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoRecommendation.RepoRecommendation;
import com.google.android.things.contrib.driver.button.Button;
import com.google.android.things.contrib.driver.button.ButtonInputDriver;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

import okhttp3.Headers;
import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.RequestBody;
import okhttp3.ResponseBody;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class Recommendation extends AppCompatActivity {
    private static final String TAG = Recommendation.class.getSimpleName();
    final static int CAPTURE = 1, RECOMMENDATION = 2;

    private ApplicationClass global;

    private ImageView recommendation;

    private ButtonInputDriver reShotButton, applyButton, cancelButton;

    private Handler mCameraHandler;
    private HandlerThread mCameraThread;

    private Camera mCamera;

    private Bitmap captureBitmap, recommendationBitmap;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.recommendation);

        global = (ApplicationClass)getApplicationContext();

        recommendation = (ImageView)findViewById(R.id.recommendation);

        captureBitmap = null;
        recommendationBitmap = null;

        try{
            reShotButton = new ButtonInputDriver(
                    "BCM18",
                    Button.LogicState.PRESSED_WHEN_HIGH,
                    KeyEvent.KEYCODE_0);
            reShotButton.register();
        }catch (IOException e){}

        try{
            applyButton = new ButtonInputDriver(
                    "BCM23",
                    Button.LogicState.PRESSED_WHEN_HIGH,
                    KeyEvent.KEYCODE_1);
            applyButton.register();
        }catch (IOException e){}

        try{
            cancelButton = new ButtonInputDriver(
                    "BCM24",
                    Button.LogicState.PRESSED_WHEN_HIGH,
                    KeyEvent.KEYCODE_2);
            cancelButton.register();
        }catch (IOException e){}

        mCameraThread = new HandlerThread("CameraBackground");
        mCameraThread.start();
        mCameraHandler = new Handler(mCameraThread.getLooper());

        mCamera = Camera.getInstance();
        mCamera.initializeCamera(this, mCameraHandler, mOnImageAvailableListener);

    }



    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_0) {

            Intent recommendation = new Intent(Recommendation.this, Streaming.class);
            startActivity(recommendation);
            this.finish();

        }

        if(keyCode == KeyEvent.KEYCODE_1){
            if(captureBitmap != null){
                ImageUpload();
                recommendationBitmap = null;
            }

        }

        if (keyCode == KeyEvent.KEYCODE_2) {

            this.finish();

        }

        return super.onKeyUp(keyCode, event);
    }

    @Override
    protected void onResume() {
        mCamera.takePicture();
        recommendation.setImageBitmap(global.captureImage);
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mCamera.shutDown();

        mCameraThread.quitSafely();
    }

    private ImageReader.OnImageAvailableListener mOnImageAvailableListener =
            new ImageReader.OnImageAvailableListener() {
                @Override
                public void onImageAvailable(ImageReader reader) {
                    Image image = reader.acquireLatestImage();
                    ByteBuffer imageBuf = image.getPlanes()[0].getBuffer();
                    final byte[] imageBytes = new byte[imageBuf.remaining()];
                    imageBuf.get(imageBytes);
                    image.close();

                    captureBitmap = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.length);
                    handler.sendEmptyMessage(CAPTURE);
                }
            };

    Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            if(msg.what == CAPTURE){
                Matrix sideInversion = new Matrix();
                sideInversion.setScale(-1, 1);
                captureBitmap = Bitmap.createBitmap(captureBitmap, 0, 0,
                        captureBitmap.getWidth(), captureBitmap.getHeight(), sideInversion, false);
                recommendation.setImageBitmap(captureBitmap);
            }else if(msg.what == RECOMMENDATION){
                recommendation.setImageBitmap(recommendationBitmap);
            }
            super.handleMessage(msg);
        }
    };

    private void ImageUpload(){

        File f = new File(getApplication().getCacheDir(), "captureImage.jpeg");
        try{
            f.createNewFile();
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            captureBitmap.compress(Bitmap.CompressFormat.JPEG, 100, bos);
            byte[] bitmapdata = bos.toByteArray();

            FileOutputStream fos = new FileOutputStream(f);
            fos.write(bitmapdata);
            fos.flush();
            fos.close();

        }catch (FileNotFoundException e){
            e.printStackTrace();
        }catch (IOException e) {
            e.printStackTrace();
        }

        RequestBody reqFile = RequestBody.create(MediaType.parse("multipart/form-data"), f);
        MultipartBody.Part body = MultipartBody.Part.createFormData("file", f.getName(), reqFile);

//        OkHttpClient.Builder clientBuilder = new OkHttpClient.Builder();
//        HttpLoggingInterceptor loggingInterceptor = new HttpLoggingInterceptor();
//        loggingInterceptor.setLevel(HttpLoggingInterceptor.Level.BODY);
//        clientBuilder.addInterceptor(loggingInterceptor);
//        레트로핏 오류 확인 ~~~.client(clientBuilder.build())

        final Retrofit recommendation = new Retrofit.Builder().baseUrl(RepoRecommendation.ApiService.API_URL_API)
                .addConverterFactory(GsonConverterFactory.create()).build();
        RepoRecommendation.ApiService recommendationService = recommendation.create(RepoRecommendation.ApiService.class);

        Call<okhttp3.ResponseBody> req =  recommendationService.post_userToken(body, "Application/json", global.userInfo.getToken(), global.userInfo.getMemberSerialNum());
        req.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {

                if(Integer.toString(response.code()).equals("200")){

//                    Headers headers = response.headers();
//                    String type = headers.get("Content-Type").toString().substring(headers.get("Content-Type").toString().lastIndexOf("/") + 1);
                    //jpg, jpeg, png 타입 판별, 굳이 사용 안해도 상관없음 혹시몰라 나둬야지

                    recommendationBitmap = BitmapFactory.decodeStream(response.body().byteStream());
                    handler.sendEmptyMessage(RECOMMENDATION);

                }else{
                    Log.i("실패", response.errorBody().toString());
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                Log.i("fail", call.toString());
                Log.i("fail", t.toString());
            }
        });

    }

}
