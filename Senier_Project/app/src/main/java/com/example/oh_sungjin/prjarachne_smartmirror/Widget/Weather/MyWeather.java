package com.example.oh_sungjin.prjarachne_smartmirror.Widget.Weather;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.oh_sungjin.prjarachne_smartmirror.R;

public class MyWeather extends LinearLayout {

    private TextView tem;
    private TextView temMinMax;
    private TextView county;

    private ImageView weatherIcon;

    private Context con;

    public MyWeather(Context context) {
        super(context);
        con = context;
        initView();
    }

    public MyWeather(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public MyWeather(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        initView();
    }

    private void getAttrs(AttributeSet attrs) {
        TypedArray typedArray = getContext().obtainStyledAttributes(attrs, R.styleable.WeatherAttrs);
        setTypeArray(typedArray);
    }


    private void getAttrs(AttributeSet attrs, int defStyle) {
        TypedArray typedArray = getContext().obtainStyledAttributes(attrs, R.styleable.WeatherAttrs, defStyle, 0);
        setTypeArray(typedArray);
    }


    private void setTypeArray(TypedArray typedArray) {
        int color_ID = typedArray.getColor(R.styleable.WatchAttrs_color, 0);

        tem.setTextColor(color_ID);
        temMinMax.setTextColor(color_ID);
        county.setTextColor(color_ID);

        typedArray.recycle();
    }

    private void initView(){

        String infService = Context.LAYOUT_INFLATER_SERVICE;
        LayoutInflater layoutInflater = (LayoutInflater)getContext().getSystemService(infService);
        View v = layoutInflater.inflate(R.layout.weather, this, false);
        addView(v);

        tem = (TextView)findViewById(R.id.tem);
        temMinMax = (TextView)findViewById(R.id.temMinMax);
        county = (TextView)findViewById(R.id.county);

        //Typeface typeface = Typeface.createFromAsset(getContext().getAssets(), "fonts/NanumGothic.ttf");
        Typeface typeface = getResources().getFont(R.font.nanumgothic);
        county.setTypeface(typeface);

        weatherIcon = (ImageView)findViewById(R.id.weather_Icon);

    }

    public TextView getTem() {
        return tem;
    }

    public void setTem(TextView tem) {
        this.tem = tem;
    }

    public TextView getTemMinMax() {
        return temMinMax;
    }

    public void setTemMinMax(TextView temMinMax) {
        this.temMinMax = temMinMax;
    }

    public ImageView getWeatherIcon() {
        return weatherIcon;
    }

    public void setWeatherIcon(ImageView weatherIcon) {
        this.weatherIcon = weatherIcon;
    }

    public TextView getCounty() {
        return county;
    }

    public void setCounty(TextView county) {
        this.county = county;
    }

    public void setCountyText(String county){
        this.county.setText(county);
    }

    public void setTemText(String tem){
        this.tem.setText(tem);
    }

    public void setTemMinMaxText(String temMinMax){
        this.temMinMax.setText(temMinMax);
    }

    public void setIcon(int iconResource){
        this.weatherIcon.setImageResource(iconResource);
    }
}
