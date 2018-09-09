package com.example.oh_sungjin.prjarachne_smartmirror.FireBase;


import android.util.Log;

import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.FirebaseInstanceIdService;

import java.io.IOException;

import okhttp3.FormBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;

public class MyFirebaseInstanceIDService extends FirebaseInstanceIdService {
    private static final String TAG = "MyFirebaseIIDService";

    /**
     * Called if InstanceID token is updated. This may occur if the security of
     * the previous token had been compromised. Note that this is called when the InstanceID token
     * is initially generated so this is where you would retrieve the token.
     */
    // [START refresh_token]
    @Override
    public void onTokenRefresh() {
        // 설치할때 여기서 토큰을 자동으로 만들어 준다
        String refreshedToken = FirebaseInstanceId.getInstance().getToken();
//        System.out.println("Refreshed token: " + refreshedToken);
        Log.i("Refreshed token: ", refreshedToken);

        // 생성한 토큰을 서버로 날려서 저장하기 위해서 만든거
        sendRegistrationToServer(refreshedToken);
    }

    private void sendRegistrationToServer(String token) {
        // Add custom implementation, as needed.

        // OKHTTP를 이용해 웹서버로 토큰값을 날려준다.
        OkHttpClient client = new OkHttpClient();
        RequestBody body = new FormBody.Builder()
                .add("token", token)
                .add("userId","osj4872@naver.com")
                .build();

        //request
        Request request = new Request.Builder()
                .url("http://49.236.136.218:80/api/v1/fcm/token")
                .post(body)
                .build();

        try {
            client.newCall(request).execute();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}
