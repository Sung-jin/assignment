package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoTodoList;

import com.google.gson.annotations.SerializedName;

public class Status {

    @SerializedName("apiType")
    private String apiType;

    @SerializedName("statusCode")
    private String statusCode;

    @SerializedName("message")
    private String message;

    public String getApiType() {
        return apiType;
    }

    public void setApiType(String apiType) {
        this.apiType = apiType;
    }

    public String getStatusCode() {
        return statusCode;
    }

    public void setStatusCode(String statusCode) {
        this.statusCode = statusCode;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

}
