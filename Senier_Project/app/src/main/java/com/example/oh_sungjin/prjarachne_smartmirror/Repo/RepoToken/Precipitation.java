package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken;

import com.google.gson.annotations.SerializedName;

public class Precipitation{

    @SerializedName("sinceOnTime")
    private String sinceOnTime;

    @SerializedName("precipitationType")
    private  String precipitationType;

    public String getSinceOnTime() {
        return sinceOnTime;
    }

    public void setSinceOnTime(String sinceOnTime) {
        this.sinceOnTime = sinceOnTime;
    }

    public String getPrecipitationType() {
        return precipitationType;
    }

    public void setPrecipitationType(String precipitationType) {
        this.precipitationType = precipitationType;
    }
}
