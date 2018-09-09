package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoRecommendation;

import okhttp3.MultipartBody;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.Header;
import retrofit2.http.Multipart;
import retrofit2.http.POST;
import retrofit2.http.Part;
import retrofit2.http.Path;

public class RepoRecommendation {

    public interface ApiService {
        public static final String API_URL_API = "http://49.236.136.218/";

        @Multipart
        @POST("api/v1/fashion/{memberId}")
        Call<ResponseBody> post_userToken(@Part MultipartBody.Part filePart, @Header("Accept") String Accept, @Header("x-auth-token") String x_auth_token, @Path("memberId") String memberId) ;

    }
}
