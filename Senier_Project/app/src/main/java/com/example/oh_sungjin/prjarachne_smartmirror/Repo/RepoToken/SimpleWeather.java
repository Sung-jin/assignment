package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken;

import com.google.gson.annotations.SerializedName;

public class SimpleWeather{

    @SerializedName("grid")
    public Grid grid;

    @SerializedName("releaseTime")
    private String releaseTime;

    @SerializedName("precipitation")
    public Precipitation precipitation;

    @SerializedName("skyState")
    private String skyState;

    @SerializedName("temperatureToday")
    private String temperatureToday;

    @SerializedName("temperatureMax")
    private String temperatureMax;

    @SerializedName("temperatureMin")
    private String temperatureMin;

    @SerializedName("humidity")
    private String humidity;

    @SerializedName("windirection")
    private String windirection;

    @SerializedName("windspeed")
    private String windspeed;

    public Grid getGrid() {
        return grid;
    }

    public void setGrid(Grid Grid) {
        this.grid = Grid;
    }

    public String getReleaseTime() {
        return releaseTime;
    }

    public void setReleaseTime(String releaseTime) {
        this.releaseTime = releaseTime;
    }

    public Precipitation getPrecipitation() {
        return precipitation;
    }

    public void setPrecipitation(Precipitation Precipitation) {
        this.precipitation = Precipitation;
    }

    public String getSkyState() {
        return skyState;
    }

    public void setSkyState(String skyState) {
        this.skyState = skyState;
    }

    public String getTemperatureToday() {
        return temperatureToday;
    }

    public void setTemperatureToday(String temperatureToday) {
        this.temperatureToday = temperatureToday;
    }

    public String getTemperatureMax() {
        return temperatureMax;
    }

    public void setTemperatureMax(String temperatureMax) {
        this.temperatureMax = temperatureMax;
    }

    public String getTemperatureMin() {
        return temperatureMin;
    }

    public void setTemperatureMin(String temperatureMin) {
        this.temperatureMin = temperatureMin;
    }

    public String getHumidity() {
        return humidity;
    }

    public void setHumidity(String humidity) {
        this.humidity = humidity;
    }

    public String getWindirection() {
        return windirection;
    }

    public void setWindirection(String windirection) {
        this.windirection = windirection;
    }

    public String getWindspeed() {
        return windspeed;
    }

    public void setWindspeed(String windspeed) {
        this.windspeed = windspeed;
    }
}
