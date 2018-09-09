package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken;

import com.example.oh_sungjin.prjarachne_smartmirror.DTO.LoginDTO;
import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.Header;
import retrofit2.http.POST;

public class RepoToken{

    @SerializedName("memberSerialNum")
    private String memberSerialNum;

    @SerializedName("userEmail")
    private String userEmail;

    @SerializedName("authorities")
    public List<Authorities> authorities = new ArrayList<>();

    @SerializedName("token")
    private String token;

    @SerializedName("mirrorSettingDTO")
    public MirrorSettingDTO mirrorSettingDTO = new MirrorSettingDTO();

    @SerializedName("simpleWeather")
    public SimpleWeather simpleWeather = new SimpleWeather();

    @SerializedName("status")
    public Status status = new Status();

    public String getMemberSerialNum() {
        return memberSerialNum;
    }

    public void setMemberSerialNum(String memberSerialNum) {
        this.memberSerialNum = memberSerialNum;
    }

    public String getUserEmail() {
        return userEmail;
    }

    public void setUserEmail(String userEmail) {
        this.userEmail = userEmail;
    }

    public List<Authorities> getAuthorities() {
        return authorities;
    }

    public void setAuthorities(List<Authorities> authorities) {
        this.authorities = authorities;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public MirrorSettingDTO getMirrorSettingDTO() {
        return mirrorSettingDTO;
    }

    public void setMirrorSettingDTO(MirrorSettingDTO mirrorSettingDTO) {
        this.mirrorSettingDTO = mirrorSettingDTO;
    }

    public SimpleWeather getSimpleWeather() {
        return simpleWeather;
    }

    public void setSimpleWeather(SimpleWeather simpleWeather) {
        this.simpleWeather = simpleWeather;
    }

    public Status getStatus() {
        return status;
    }

    public void setStatus(Status status) {
        this.status = status;
    }

    public interface ApiService {
        public static final String API_URL_AUTH = "http://49.236.136.217/";

        @POST("basic/authorize")
        Call<RepoToken> post_userToken(@Header("Accept") String Accept, @Body LoginDTO loginDTO) ;

    }
}
