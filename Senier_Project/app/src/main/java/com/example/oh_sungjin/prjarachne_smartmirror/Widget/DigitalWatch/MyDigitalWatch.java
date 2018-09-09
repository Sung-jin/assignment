package com.example.oh_sungjin.prjarachne_smartmirror.Widget.DigitalWatch;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.oh_sungjin.prjarachne_smartmirror.R;

public class MyDigitalWatch extends LinearLayout {

    private TextView timeText;
    private TextView ampmText;
    private TextView dateText;

    public MyDigitalWatch(Context context) {
        super(context);
        initView();
    }

    public MyDigitalWatch(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public MyDigitalWatch(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs);
        initView();
    }

    private void getAttrs(AttributeSet attrs) {
        TypedArray typedArray = getContext().obtainStyledAttributes(attrs, R.styleable.WatchAttrs);
        setTypeArray(typedArray);
    }


    private void getAttrs(AttributeSet attrs, int defStyle) {
        TypedArray typedArray = getContext().obtainStyledAttributes(attrs, R.styleable.WatchAttrs, defStyle, 0);
        setTypeArray(typedArray);
    }


    private void setTypeArray(TypedArray typedArray) {
        int color_ID = typedArray.getColor(R.styleable.WatchAttrs_color, 0);

        timeText.setTextColor(color_ID);
        ampmText.setTextColor(color_ID);
        dateText.setTextColor(color_ID);

        typedArray.recycle();
    }

    private void initView(){

        String infService = Context.LAYOUT_INFLATER_SERVICE;
        LayoutInflater layoutInflater = (LayoutInflater)getContext().getSystemService(infService);
        View v = layoutInflater.inflate(R.layout.digital_watch, this, false);
        addView(v);

        this.timeText = (TextView)findViewById(R.id.time);
        this.ampmText = (TextView)findViewById(R.id.ampm);
        this.dateText = (TextView)findViewById(R.id.date);

    }

    public TextView getTimeText() {
        return timeText;
    }

    public void setTimeText(TextView timeText) {
        this.timeText = timeText;
    }

    public TextView getAmpmText() {
        return ampmText;
    }

    public void setAmpmText(TextView ampmText) {
        this.ampmText = ampmText;
    }

    public TextView getDateText() {
        return dateText;
    }

    public void setDateText(TextView dateText) {
        this.dateText = dateText;
    }

    public void setTime(String time){
        timeText.setText(time);
    }

    public void setAmPm(String ampm){
        ampmText.setText(ampm);
    }

    public void setDate(String Date){
        dateText.setText(Date);
    }
}
