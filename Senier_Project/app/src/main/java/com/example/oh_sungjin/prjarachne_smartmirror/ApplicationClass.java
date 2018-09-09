package com.example.oh_sungjin.prjarachne_smartmirror;

import android.app.AlertDialog;
import android.app.Application;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.widget.LinearLayout;

import com.example.oh_sungjin.prjarachne_smartmirror.DTO.LoginDTO;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoTodoList.Entity;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoTodoList.RepoTodoList;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoToken.RepoToken;
import com.example.oh_sungjin.prjarachne_smartmirror.Widget.DigitalWatch.MyDigitalWatch;
import com.example.oh_sungjin.prjarachne_smartmirror.Widget.TodoList.MyTodoList;
import com.example.oh_sungjin.prjarachne_smartmirror.Widget.Weather.MyWeather;
import com.prolificinteractive.materialcalendarview.MaterialCalendarView;
import com.tsengvn.typekit.Typekit;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.TimeZone;

import retrofit2.Retrofit;


public class ApplicationClass extends Application {

    public int renewal = 0;

    public int deleteCnt = 0, deleteDoneCnt = 0;
    public boolean deleteCheck = true;

    public AlertDialog.Builder notice;

    public String city, county, district;

    public MaterialCalendarView calendarView;
    public MyTodoList todoListView;
    public MyDigitalWatch watchView;
    public MyWeather weatherView;

    public LinearLayout area[] = new LinearLayout[9];

    public HashMap<String, Integer> weatherIcon;

    public RepoToken userInfo = new RepoToken();
    public RepoToken beforeUserInfo = new RepoToken();
    public RepoTodoList todoList = new RepoTodoList();

    public List<Entity> entity = new ArrayList<>();

    public Retrofit tokenRetrofit, todoListRetrofit;

    public RepoToken.ApiService tokenService;
    public RepoTodoList.ApiService todoListService;

    public TimeZone timeZone;

    public String time;
    public String ampm;
    public String date;

    public SimpleDateFormat timeDate;
    public SimpleDateFormat ampmDate;
    public SimpleDateFormat dayFormatDate;

    public Calendar calendar;

    public LoginDTO loginDTO;

    public Bitmap captureImage = null;

    public String FCMstring;

    @Override
    public void onCreate() {
        super.onCreate();

        Typekit.getInstance().
                addNormal(Typekit.createFromAsset(this, "fonts/NanumGothic.ttf"));

    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }

}
