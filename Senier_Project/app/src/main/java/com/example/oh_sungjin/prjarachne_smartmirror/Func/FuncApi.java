package com.example.oh_sungjin.prjarachne_smartmirror.Func;

import android.content.Context;
import android.util.Log;
import android.view.View;

import com.example.oh_sungjin.prjarachne_smartmirror.ApplicationClass;
import com.example.oh_sungjin.prjarachne_smartmirror.DTO.LoginDTO;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken.RepoToken;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class FuncApi extends View{

    private ApplicationClass applicationClass;

    public FuncApi(Context context){
        super(context);
        applicationClass = (ApplicationClass)context.getApplicationContext();
    }

    public void getToken(RepoToken.ApiService tokenService, LoginDTO loginDTO){

        Call<RepoToken> getToken = tokenService.post_userToken("application/json", loginDTO);
        getToken.enqueue(new Callback<RepoToken>(){
            @Override
            public void onResponse(Call<RepoToken> call, Response<RepoToken> response) {
                if(response.isSuccessful()){
                    applicationClass.userInfo = response.body();
                    applicationClass.renewal = 1;
                }
            }

            @Override
            public void onFailure(Call<RepoToken> call, Throwable t) {
                Log.i("Repo", t.getMessage());
                Log.i("Repo", call.request().toString());
            }
        });
    }

}
