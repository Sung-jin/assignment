package com.example.oh_sungjin.prjarachne_smartmirror;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.Toast;

import com.example.oh_sungjin.prjarachne_smartmirror.DTO.LoginDTO;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoTodoList.RepoTodoList;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken.RepoToken;
import com.tsengvn.typekit.TypekitContextWrapper;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.Locale;
import java.util.TimeZone;
import java.util.concurrent.TimeUnit;

import okhttp3.OkHttpClient;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class InitActivity extends AppCompatActivity {

    private ApplicationClass global;

    private ProgressDialog init;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        global = (ApplicationClass)getApplicationContext();

        global.notice = new AlertDialog.Builder(InitActivity.this);

        global.weatherIcon = new HashMap<String, Integer>();

        global.weatherIcon.put("맑음", R.drawable.weather_01);
        global.weatherIcon.put("구름조금", R.drawable.weather_02);
        global.weatherIcon.put("구름많음", R.drawable.weather_03);
        global.weatherIcon.put("구름많고 비", R.drawable.weather_04);
        global.weatherIcon.put("구름많고 눈", R.drawable.weather_05);
        global.weatherIcon.put("구름많고 비 또는 눈", R.drawable.weather_06);
        global.weatherIcon.put("흐림", R.drawable.weather_07);
        global.weatherIcon.put("흐리고 비", R.drawable.weather_08);
        global.weatherIcon.put("흐리고 눈", R.drawable.weather_09);
        global.weatherIcon.put("흐리고 비 또는 눈", R.drawable.weather_10);
        global.weatherIcon.put("흐리고 낙뢰", R.drawable.weather_11);
        global.weatherIcon.put("뇌우, 비", R.drawable.weather_12);
        global.weatherIcon.put("뇌우, 눈", R.drawable.weather_13);
        global.weatherIcon.put("뇌우, 비 또는 눈", R.drawable.weather_14);

        global.timeDate = new SimpleDateFormat("hh:mm");
        global.ampmDate = new SimpleDateFormat("aa");
        global.dayFormatDate = new SimpleDateFormat("EEE, MMM, dd", Locale.US);
        global.timeZone = TimeZone.getTimeZone("Asia/Seoul");

        global.timeDate.setTimeZone(global.timeZone);
        global.ampmDate.setTimeZone(global.timeZone);
        global.dayFormatDate.setTimeZone(global.timeZone);

        final OkHttpClient okHttpClient =
                new OkHttpClient.Builder()
                .readTimeout(60, TimeUnit.SECONDS)
                .connectTimeout(60, TimeUnit.SECONDS)
                .build();


        global.tokenRetrofit = new Retrofit.Builder().baseUrl(RepoToken.ApiService.API_URL_AUTH).client(okHttpClient)
                .addConverterFactory(GsonConverterFactory.create()).build();
        global.tokenService = global.tokenRetrofit.create(RepoToken.ApiService.class);

        global.todoListRetrofit = new Retrofit.Builder().baseUrl(RepoTodoList.ApiService.API_URL_API).client(okHttpClient)
                .addConverterFactory(GsonConverterFactory.create()).build();
        global.todoListService = global.todoListRetrofit.create(RepoTodoList.ApiService.class);

        try{

            global.loginDTO = new LoginDTO();

            File file = new File(getFilesDir() + "info.txt");

            if(file.exists() == false){
                BufferedWriter bw = new BufferedWriter(new FileWriter(getFilesDir() + "info.txt"));
                bw.write("osj4872@naver.com\n");
                bw.write("1234");
                bw.close();
            }

            BufferedReader br = new BufferedReader(new FileReader(getFilesDir() + "info.txt"));

            if(file.exists()){
                global.loginDTO.setUserEmail(br.readLine());
                global.loginDTO.setPassword(br.readLine());
            }else{
                global.notice.setTitle("Initialize Error.")
                        .setMessage("프로그램을 종료합니다.")
                        .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                InitActivity.this.finish();
                            }
                        });
            }

            br.close();

        }catch(FileNotFoundException e){
            e.printStackTrace();
        }catch (IOException e){
            e.printStackTrace();
        }

        init = ProgressDialog.show(InitActivity.this, "Wait please...", "잠시만 기다려 주세요..");

        //스마트 미러 사용자 초기화 - 사용자 토큰 가져오기
        Call<RepoToken> initialize = global.tokenService.post_userToken("application/json", global.loginDTO);
        initialize.enqueue(new Callback<RepoToken>() {
            @Override
            public void onResponse(Call<RepoToken> call, Response<RepoToken> response) {
                if(response.isSuccessful()){

                    global.userInfo = response.body();

                    Call<RepoTodoList> getTodoList = global.todoListService.getTodoList("application/json", global.userInfo.getToken(), global.userInfo.getMemberSerialNum());
                    getTodoList.enqueue(new Callback<RepoTodoList>() {
                        @Override
                        public void onResponse(Call<RepoTodoList> call, Response<RepoTodoList> response) {
                            if(response.isSuccessful()){
                                global.todoList = response.body();
                                global.entity = global.todoList.entity;

                                init.dismiss();
                                Intent mirror = new Intent(InitActivity.this, MirrorActivity.class);
                                startActivity(mirror);
                            }else{
                                init.dismiss();
                                Toast.makeText(getApplication(), "초기화 실패, 인터넷 환경을 확인 해 주세요.", Toast.LENGTH_LONG).show();
                                System.exit(0);
                            }
                        }

                        @Override
                        public void onFailure(Call<RepoTodoList> call, Throwable t) {
                            init.dismiss();
                            Toast.makeText(getApplicationContext(), "초기화 실패, 인터넷 환경을 확인 해 주세요.", Toast.LENGTH_SHORT).show();
                            Log.i("fail", call.request().headers().toString());
                            Log.i("fail", t.toString());
                            System.exit(0);
                        }
                    });

                }else{
                    init.dismiss();
                    Toast.makeText(getApplication(), "초기화 실패, 인터넷 환경을 확인 해 주세요.", Toast.LENGTH_LONG).show();
                    System.exit(0);
                }
            }

            @Override
            public void onFailure(Call<RepoToken> call, Throwable t) {
                init.dismiss();
                Toast.makeText(getApplicationContext(), "초기화 실패, 인터넷 환경을 확인 해 주세요.", Toast.LENGTH_SHORT).show();
                Log.i("fail", call.request().headers().toString());
                Log.i("fail", t.toString());
                System.exit(0);
            }
        });


    }

    @Override
    protected void attachBaseContext(Context newBase) {
        super.attachBaseContext(TypekitContextWrapper.wrap(newBase));
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        init.dismiss();
    }



}
