package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken;

import com.google.gson.annotations.SerializedName;

public class Grid{
    @SerializedName("village")
    private String village;

    @SerializedName("city")
    private String city;

    @SerializedName("country")
    private String country;

    @SerializedName("latitude")
    private String latitude;

    @SerializedName("longtitude")
    private String longtitude;

    public String getVillage() {
        return village;
    }

    public void setVillage(String village) {
        this.village = village;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    public String getLatitude() {
        return latitude;
    }

    public void setLatitude(String latitude) {
        this.latitude = latitude;
    }

    public String getLongtitude() {
        return longtitude;
    }

    public void setLongtitude(String longtitude) {
        this.longtitude = longtitude;
    }
}
