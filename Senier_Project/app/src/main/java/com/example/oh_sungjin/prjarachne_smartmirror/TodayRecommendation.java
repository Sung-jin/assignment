package com.example.oh_sungjin.prjarachne_smartmirror;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.KeyEvent;
import android.widget.ImageView;

import com.google.android.things.contrib.driver.button.Button;
import com.google.android.things.contrib.driver.button.ButtonInputDriver;

import java.io.IOException;

public class TodayRecommendation extends AppCompatActivity {

    final static int WARM = 1, WARM_RAINY = 2, HOT = 3, HOT_RAINY = 4, COOL = 5, COOL_RAINY = 6, COOL_SNOW = 7, COLD = 8, COLD_RAINY = 9, COLD_SNOW = 10;

    private ApplicationClass global;

    private ImageView recommendation_1, recommendation_2, recommendation_3;

    private ButtonInputDriver shotButton, cancelButton;

    private String sky;
    private int todayState;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.today_recommendation);

        global = (ApplicationClass)getApplicationContext();

        recommendation_1 = (ImageView)findViewById(R.id.recommendation_1);
        recommendation_2 = (ImageView)findViewById(R.id.recommendation_2);
        recommendation_3 = (ImageView)findViewById(R.id.recommendation_3);

        sky = global.userInfo.simpleWeather.getSkyState();
        //봄 - 3~13도, 여름 - 20도 이상, 가을 - 13-20도, 겨울 - 3도이하

        if(Double.parseDouble(global.userInfo.simpleWeather.getTemperatureToday()) <= 3){
            //겨울

            if(sky.equals("맑음") || sky.equals("구름조금") || sky.equals("구름많음") || sky.equals("흐림")){
                todayState = COLD;
            }else if(sky.equals("구름많고 비") || sky.equals("구름많고 비 또는 눈") || sky.equals("흐리고 비")
                    || sky.equals("흐리고 눈") || sky.equals("흐리고 비 또는 눈") || sky.equals("흐리고 낙뢰") || sky.equals("뇌우, 비")
                    || sky.equals("뇌우, 눈") || sky.equals("뇌우, 비 또는 눈")){
                todayState = COLD_RAINY;
            }else if(sky.equals("구름많고 눈") || sky.equals("구름많고 비 또는 눈") || sky.equals("흐리고 눈") || sky.equals("흐리고 비 또는 눈")
                    || sky.equals("뇌우, 눈") || sky.equals("뇌우, 비 또는 눈")){
                todayState = COLD_SNOW;
            }
        }else if(Double.parseDouble(global.userInfo.simpleWeather.getTemperatureToday()) > 3 && Double.parseDouble(global.userInfo.simpleWeather.getTemperatureToday()) < 13){
            //봄

            if(sky.equals("맑음") || sky.equals("구름조금") || sky.equals("구름많음") || sky.equals("흐림")){
                todayState = WARM;
            }else if(sky.equals("구름많고 비") || sky.equals("구름많고 비 또는 눈") || sky.equals("흐리고 비")
                    || sky.equals("흐리고 눈") || sky.equals("흐리고 비 또는 눈") || sky.equals("흐리고 낙뢰") || sky.equals("뇌우, 비")
                    || sky.equals("뇌우, 눈") || sky.equals("뇌우, 비 또는 눈")){
                todayState = WARM_RAINY;
            }
        }else if(Double.parseDouble(global.userInfo.simpleWeather.getTemperatureToday()) >= 13 && Double.parseDouble(global.userInfo.simpleWeather.getTemperatureToday()) < 20){
            //가을

            if(sky.equals("맑음") || sky.equals("구름조금") || sky.equals("구름많음") || sky.equals("흐림")){
                todayState = COOL;
            }else if(sky.equals("구름많고 비") || sky.equals("구름많고 비 또는 눈") || sky.equals("흐리고 비")
                    || sky.equals("흐리고 눈") || sky.equals("흐리고 비 또는 눈") || sky.equals("흐리고 낙뢰") || sky.equals("뇌우, 비")
                    || sky.equals("뇌우, 눈") || sky.equals("뇌우, 비 또는 눈")){
                todayState = COOL_RAINY;
            }else if(sky.equals("구름많고 눈") || sky.equals("구름많고 비 또는 눈") || sky.equals("흐리고 눈") || sky.equals("흐리고 비 또는 눈")
                    || sky.equals("뇌우, 눈") || sky.equals("뇌우, 비 또는 눈")){
                todayState = COOL_SNOW;
            }
        }if(Double.parseDouble(global.userInfo.simpleWeather.getTemperatureToday()) > 20){
            //겨울

            if(sky.equals("맑음") || sky.equals("구름조금") || sky.equals("구름많음") || sky.equals("흐림")){
                todayState = HOT;
            }else if(sky.equals("구름많고 비") || sky.equals("구름많고 비 또는 눈") || sky.equals("흐리고 비")
                    || sky.equals("흐리고 눈") || sky.equals("흐리고 비 또는 눈") || sky.equals("흐리고 낙뢰") || sky.equals("뇌우, 비")
                    || sky.equals("뇌우, 눈") || sky.equals("뇌우, 비 또는 눈")){
                todayState = HOT_RAINY;
            }
        }

        try{
            shotButton = new ButtonInputDriver(
                    "BCM18",
                    Button.LogicState.PRESSED_WHEN_HIGH,
                    KeyEvent.KEYCODE_0);
            shotButton.register();
        }catch (IOException e){}

        try{
            cancelButton = new ButtonInputDriver(
                    "BCM24",
                    Button.LogicState.PRESSED_WHEN_HIGH,
                    KeyEvent.KEYCODE_2);
            cancelButton.register();
        }catch (IOException e){}

    }

    @Override
    protected void onResume() {
        super.onResume();

        //여기에서 일단 성렬씨가 추천알고리즘 완성시킬시 날씨에 따라 이미지 3개 추천
        //알고리즘 이미지가 충분하지 않으면 적절한 이미지 넣어놓고 돌려사용

        //1920×1080
        //640 / 1080

        Bitmap orgImage = BitmapFactory.decodeResource(getResources(), R.drawable.test_image);
        Bitmap resize = Bitmap.createScaledBitmap(orgImage, 640, 1080, true);
        recommendation_1.setImageBitmap(resize);

        orgImage = BitmapFactory.decodeResource(getResources(), R.drawable.test_image2);
        resize = Bitmap.createScaledBitmap(orgImage, 640, 1080, true);
        recommendation_2.setImageBitmap(resize);

        orgImage = BitmapFactory.decodeResource(getResources(), R.drawable.test_image3);
        resize = Bitmap.createScaledBitmap(orgImage, 640, 1080, true);
        recommendation_3.setImageBitmap(resize);

//        recommendation_1.setImageResource(R.drawable.test_image);
//        recommendation_2.setImageResource(R.drawable.test_image2);
//        recommendation_3.setImageResource(R.drawable.test_image3);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_0) {

            Intent recommendation = new Intent(TodayRecommendation.this, Streaming.class);
            startActivity(recommendation);
            this.finish();

        }

        if (keyCode == KeyEvent.KEYCODE_2) {

            this.finish();

        }

        return super.onKeyUp(keyCode, event);
    }

}
