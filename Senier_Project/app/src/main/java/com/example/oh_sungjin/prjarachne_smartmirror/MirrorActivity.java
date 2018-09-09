package com.example.oh_sungjin.prjarachne_smartmirror;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.LinearLayout;

import com.example.oh_sungjin.prjarachne_smartmirror.Func.FuncApi;
import com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoTodoList.RepoTodoList;
import com.example.oh_sungjin.prjarachne_smartmirror.Widget.Calendar.MyCalendar;
import com.example.oh_sungjin.prjarachne_smartmirror.Widget.DigitalWatch.MyDigitalWatch;
import com.example.oh_sungjin.prjarachne_smartmirror.Widget.TodoList.MyTodoList;
import com.example.oh_sungjin.prjarachne_smartmirror.Widget.Weather.MyWeather;
import com.google.android.things.contrib.driver.button.Button;
import com.google.android.things.contrib.driver.button.ButtonInputDriver;
import com.tsengvn.typekit.TypekitContextWrapper;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MirrorActivity extends AppCompatActivity {

    final static int renewalWeather = 1, renewalCalendar = 2, renewalTodo = 3, renewalWatch = 4, initView = 5, renewalView = 6;
    private Boolean initCheck = true;

    private ApplicationClass global;

    private FuncApi funcApi;

    private MyCalendar myCalendar;

    private int renewalTime, nextRenewalTime;

    private ButtonInputDriver shotButton;

    private TimeThread timeThread;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mirror);

        global = (ApplicationClass)getApplicationContext();

        for(int i = 0; i < 9; i++){
            int resID = getResources().getIdentifier("space" + (i + 1), "id", "com.example.oh_sungjin.prjarachne_smartmirror");
            global.area[i] = (LinearLayout)findViewById(resID);
        }

        funcApi = new FuncApi(getApplicationContext());

        try{
            shotButton = new ButtonInputDriver(
                    "BCM18",
                    Button.LogicState.PRESSED_WHEN_HIGH,
                    KeyEvent.KEYCODE_0);
            shotButton.register();
        }catch (IOException e){
            e.printStackTrace();
        }

        timeThread = new TimeThread();
        timeThread.execute();

    }

    @Override
    protected void onResume() {

        try{
            if (timeThread.getStatus() == AsyncTask.Status.FINISHED){
                timeThread = new TimeThread();
                timeThread.execute();
            }else{}
        }catch (Exception e){
            e.printStackTrace();
        }

        super.onResume();
    }

    @Override
    protected void onPause() {

        try{
            if (timeThread.getStatus() == AsyncTask.Status.RUNNING){
                timeThread.cancel(false);
            }else{}
        }catch (Exception e){
            e.printStackTrace();
        }

        super.onPause();
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if(keyCode == KeyEvent.KEYCODE_0){
            Intent todayRecommendation = new Intent(MirrorActivity.this, TodayRecommendation.class);
            startActivity(todayRecommendation);
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }

    @Override
    protected void attachBaseContext(Context newBase) {
        super.attachBaseContext(TypekitContextWrapper.wrap(newBase));
    }

    Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            if(msg.what == renewalWeather){
                int maxLength = global.userInfo.simpleWeather.getTemperatureMax().length();
                int minLength = global.userInfo.simpleWeather.getTemperatureMin().length();
                int todayLength = global.userInfo.simpleWeather.getTemperatureToday().length();

                String todayTem = global.userInfo.simpleWeather.getTemperatureToday().substring(0, todayLength - 3);
                String maxTem = global.userInfo.simpleWeather.getTemperatureMax().substring(0, maxLength - 3);
                String minTem = global.userInfo.simpleWeather.getTemperatureMin().substring(0, minLength - 3);

                global.weatherView.setIcon(global.weatherIcon.get(global.userInfo.simpleWeather.getSkyState()));
                global.weatherView.setCountyText(global.userInfo.simpleWeather.grid.getCountry());
                global.weatherView.setTemText(todayTem + "°");
                global.weatherView.setTemMinMaxText(maxTem + "°/" + minTem + "°");

            }else if(msg.what == renewalCalendar){
                global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosCalendar()) - 1].removeView(global.calendarView);

                myCalendar = new MyCalendar(getApplicationContext());
                global.calendarView = myCalendar.getCalView();

                global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosCalendar()) - 1].addView(global.calendarView);
            }else if(msg.what == renewalTodo){
                global.deleteCnt = 0;
                global.deleteDoneCnt = 0;
                global.deleteCheck = true;
                global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosTodoList()) - 1].removeView(global.todoListView);

                global.todoListView = new MyTodoList(getApplicationContext());
                for(int i = 0; i < global.entity.size(); i++){
                    String[] todoDate = global.entity.get(i).getDate().split(" ");
                    todoDate[1] = todoDate[1].substring(0,5);
                    global.todoListView.setTodoListData(todoDate[0], todoDate[1], global.entity.get(i).getTitle(), global.entity.get(i).getContents());
                }

                global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosTodoList()) - 1].addView(global.todoListView);
            }else if(msg.what == renewalWatch){
                global.watchView.setTime(global.time);
                global.watchView.setAmPm(global.ampm);
                global.watchView.setDate(global.date);
            }else if(msg.what == initView){

                initCheck = false;

                if(Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosWeather()) != 0){
                    handler.sendEmptyMessage(renewalWeather);
                    global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosWeather()) - 1].addView(global.weatherView);
                }

                if(Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosCalendar()) != 0){
                    global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosCalendar()) - 1].addView(global.calendarView);
                }

                if(Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosTodoList()) != 0){
                    global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosTodoList()) - 1].addView(global.todoListView);
                }

                if(Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosWatch()) != 0){
                    global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosWatch()) - 1].addView(global.watchView);
                }
            }else if(msg.what == renewalView){

                global.area[Integer.parseInt(global.beforeUserInfo.mirrorSettingDTO.getPosWeather()) - 1].removeView(global.weatherView);
                global.area[Integer.parseInt(global.beforeUserInfo.mirrorSettingDTO.getPosCalendar()) - 1].removeView(global.calendarView);
                global.area[Integer.parseInt(global.beforeUserInfo.mirrorSettingDTO.getPosTodoList()) - 1].removeView(global.todoListView);
                global.area[Integer.parseInt(global.beforeUserInfo.mirrorSettingDTO.getPosWatch()) - 1].removeView(global.watchView);

                handler.sendEmptyMessage(renewalWeather);
                handler.sendEmptyMessage(renewalCalendar);
                handler.sendEmptyMessage(renewalTodo);
                handler.sendEmptyMessage(renewalWatch);

                global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosWeather()) - 1].addView(global.weatherView);
                global.area[Integer.parseInt(global.userInfo.mirrorSettingDTO.getPosWatch()) - 1].addView(global.watchView);

            }

            super.handleMessage(msg);
        }
    };

    class TimeThread extends AsyncTask<Integer, Integer, Integer>{
        @Override
        protected Integer doInBackground(Integer... integers) {
            while(global.time != null){
                publishProgress();
                try{
                    Thread.sleep(100);
                }catch (InterruptedException e){
                    e.printStackTrace();
                }

                if (this.isCancelled()) {
                    return null;
                }
            }
            return null;
        }

        @Override
        protected void onCancelled(Integer integer) {
            super.onCancelled(integer);
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();

            if(initCheck){
                global.beforeUserInfo = global.userInfo;

                global.weatherView = new MyWeather(getApplicationContext());

                myCalendar = new MyCalendar(getApplicationContext());
                global.calendarView = myCalendar.getCalView();

                global.todoListView = new MyTodoList(getApplicationContext());

                global.watchView = new MyDigitalWatch(getApplicationContext());

                global.time = global.timeDate.format(Calendar.getInstance().getTime());
                global.ampm = global.ampmDate.format(Calendar.getInstance().getTime());
                global.date = global.dayFormatDate.format(Calendar.getInstance().getTime());

                global.calendar = Calendar.getInstance();

                handler.sendEmptyMessage(initView);

                for(int i = 0; i < global.entity.size(); i++){
                    String[] todoDate = global.entity.get(i).getDate().split(" ");
                    todoDate[1] = todoDate[1].substring(0,5);
                    global.todoListView.setTodoListData(todoDate[0], todoDate[1], global.entity.get(i).getTitle(), global.entity.get(i).getContents());
                }
            }

            renewalTime = Integer.parseInt(global.time.substring(0,2));
            nextRenewalTime = renewalTime + 1;
            if(nextRenewalTime == 13){
                nextRenewalTime = 1;
            }
        }

        @Override
        protected void onPostExecute(Integer integer) {
            super.onPostExecute(integer);
        }

        @Override
        protected void onProgressUpdate(Integer... values) {
            super.onProgressUpdate(values);

            if(global.FCMstring != null){
                if(global.FCMstring.equals("POS")){
                    global.FCMstring = null;

                    global.beforeUserInfo = global.userInfo;

                    funcApi.getToken(global.tokenService, global.loginDTO);
                }else if(global.FCMstring.equals("TODOLIST")){
                    global.FCMstring = null;

                    Call<RepoTodoList> getTodoList = global.todoListService.getTodoList("application/json", global.userInfo.getToken(), global.userInfo.getMemberSerialNum());
                    getTodoList.enqueue(new Callback<RepoTodoList>() {
                        @Override
                        public void onResponse(Call<RepoTodoList> call, Response<RepoTodoList> response) {
                            if(response.isSuccessful()){
                                global.todoList = response.body();
                                global.entity = global.todoList.entity;

                                handler.sendEmptyMessage(renewalTodo);
                            }else{
                                Log.i("fail", call.request().body().toString());
                            }
                        }

                        @Override
                        public void onFailure(Call<RepoTodoList> call, Throwable t) {
                            Log.i("fail", call.request().body().toString());
                            Log.i("fail", t.toString());
                        }
                    });
                }
            }

            global.time = global.timeDate.format(Calendar.getInstance().getTime());
            global.ampm = global.ampmDate.format(Calendar.getInstance().getTime());
            global.date = global.dayFormatDate.format(Calendar.getInstance().getTime());

            renewalTime = Integer.parseInt(global.time.substring(0,2));

            if(!global.entity.isEmpty()){
                try{
                    SimpleDateFormat nowDate = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    TimeZone tz = TimeZone.getTimeZone("Asia/Seoul");
                    nowDate.setTimeZone(tz);
                    String nowTimeString = nowDate.format(Calendar.getInstance().getTime());

                    Date nowTime = nowDate.parse(nowTimeString);
                    Date todoTime = nowDate.parse(global.entity.get(0).getDate());

                    Long now = nowTime.getTime();
                    Long todo = todoTime.getTime();

                    if(now > todo && global.deleteCheck){
                        //제일 최근 투두리스트가 시간이 지남. 삭제 처리를 여기서 해야함.
                        //삭제를 할 때 같은 시간이 여러개 잇을 수 있으니까 삭제해야 할 시간과 같은 모든 할일을 찾아서 삭제해야함.

                        global.deleteCheck = false;

                        for(int i = 0; i < global.entity.size(); i++) {
                            todoTime = nowDate.parse(global.entity.get(i).getDate());

                            todo = todoTime.getTime();
                            if (now > todo) {
                                //여기에서가 지금 시간을 기준으로 지난 투두리스트이므로 삭제처리.
                                global.deleteCnt++;

                                Call<RepoTodoList> deleteTodo = global.todoListService.deleteTodoList("application/json", global.userInfo.getToken(), global.entity.get(i).getId());
                                deleteTodo.enqueue(new Callback<RepoTodoList>() {
                                    @Override
                                    public void onResponse(Call<RepoTodoList> call, Response<RepoTodoList> response) {
                                        if(response.isSuccessful()){
                                            global.deleteDoneCnt++;
                                        }else{
                                            Log.i("Fail", "Fail to delete");
                                        }
                                    }

                                    @Override
                                    public void onFailure(Call<RepoTodoList> call, Throwable t) {
                                        Log.i("Fail", call.request().body().toString());
                                        Log.i("Fail", t.toString());
                                    }
                                });
                            } else {
                                break;
                            }
                        }
                    }
                }catch (Exception e){
                    e.printStackTrace();
                }
            }

            if(global.deleteCnt == global.deleteDoneCnt && global.deleteCnt > 0){
                Call<RepoTodoList> getTodoList = global.todoListService.getTodoList("application/json", global.userInfo.getToken(), global.userInfo.getMemberSerialNum());
                getTodoList.enqueue(new Callback<RepoTodoList>() {
                    @Override
                    public void onResponse(Call<RepoTodoList> call, Response<RepoTodoList> response) {
                        if(response.isSuccessful()){
                            global.todoList = response.body();
                            global.entity = global.todoList.entity;
                            handler.sendEmptyMessage(renewalTodo);
                        }else{
                            Log.i("Fail", "Fail to update");
                        }
                    }

                    @Override
                    public void onFailure(Call<RepoTodoList> call, Throwable t) {
                        Log.i("fail", call.request().body().toString());
                        Log.i("fail", t.toString());
                    }
                });

            }

            if(global.renewal == 1){
                handler.sendEmptyMessage(renewalView);
                global.renewal = 0;
            }

            Calendar cal = Calendar.getInstance();
            if(cal.get(Calendar.DAY_OF_WEEK) != global.calendar.get(Calendar.DAY_OF_WEEK)){
                global.calendar = Calendar.getInstance();
                handler.sendEmptyMessage(renewalCalendar);
            }

            if(renewalTime == nextRenewalTime && Integer.parseInt(global.time.substring(3,5)) == 5){
                //정각 5분마다 날씨 갱신
                //SKT weather 에서 바로바로 갱신되지 않아서 정각에서 5분뒤에 갱신해줌.
                nextRenewalTime += 1;

                if(nextRenewalTime == 13){
                    nextRenewalTime = 1;
                }

                funcApi.getToken(global.tokenService, global.loginDTO);
            }

            handler.sendEmptyMessage(renewalWatch);
        }
    }
}
