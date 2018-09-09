package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoTodoList;

import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.http.DELETE;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Path;

public class RepoTodoList {

    @SerializedName("entity")
    public List<Entity> entity = new ArrayList<>();

    @SerializedName("status")
    public Status mirrorSettingDTO = new Status();

    public List<Entity> getAuthorities() {
        return entity;
    }

    public void setAuthorities(List<Entity> authorities) {
        this.entity = authorities;
    }

    public Status getMirrorSettingDTO() {
        return mirrorSettingDTO;
    }

    public void setMirrorSettingDTO(Status mirrorSettingDTO) {
        this.mirrorSettingDTO = mirrorSettingDTO;
    }

    public interface ApiService {
        public static final String API_URL_API = "http://49.236.136.218/";

        @GET("api/v1/schedule/{memberSerialNum}")
        Call<RepoTodoList> getTodoList(@Header("Accept") String Accept, @Header("x-auth-token") String x_auth_toke, @Path("memberSerialNum") String memberSerialNum) ;

        @DELETE("api/v1/schedule/{todoItemId}")
        Call<RepoTodoList> deleteTodoList(@Header("Accept") String Accept, @Header("x-auth-token") String x_auth_toke, @Path("todoItemId") String todoItemId);

    }
}
