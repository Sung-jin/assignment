package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken;

import com.google.gson.annotations.SerializedName;

public class MirrorSettingDTO {
    @SerializedName("posWeather")
    private String posWeather;

    @SerializedName("posCalendar")
    private String posCalendar;

    @SerializedName("posTodoList")
    private String posTodoList;

    @SerializedName("posCosRecom")
    private String posCosRecom;

    @SerializedName("posNews")
    private String posNews;

    @SerializedName("posWatch")
    private String posWatch;

    @SerializedName("city")
    private String city;

    @SerializedName("county")
    private String county;

    @SerializedName("district")
    private String district;

    @SerializedName("latitude")
    private String latitude;

    @SerializedName("longitude")
    private String longitude;

    public String getPosWeather() {
        return posWeather;
    }

    public void setPosWeather(String posWeather) {
        this.posWeather = posWeather;
    }

    public String getPosCalendar() {
        return posCalendar;
    }

    public void setPosCalendar(String posCalendar) {
        this.posCalendar = posCalendar;
    }

    public String getPosTodoList() {
        return posTodoList;
    }

    public void setPosTodoList(String posTodoList) {
        this.posTodoList = posTodoList;
    }

    public String getPosCosRecom() {
        return posCosRecom;
    }

    public void setPosCosRecom(String posCosRecom) {
        this.posCosRecom = posCosRecom;
    }

    public String getPosNews() {
        return posNews;
    }

    public void setPosNews(String posNews) {
        this.posNews = posNews;
    }

    public String getPosWatch() {
        return posWatch;
    }

    public void setPosWatch(String posWatch) {
        this.posWatch = posWatch;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getCounty() {
        return county;
    }

    public void setCounty(String county) {
        this.county = county;
    }

    public String getDistrict() {
        return district;
    }

    public void setDistrict(String district) {
        this.district = district;
    }

    public String getLatitude() {
        return latitude;
    }

    public void setLatitude(String latitude) {
        this.latitude = latitude;
    }

    public String getLongitude() {
        return longitude;
    }

    public void setLongitude(String longitude) {
        this.longitude = longitude;
    }
}
