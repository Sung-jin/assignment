package com.example.oh_sungjin.prjarachne_smartmirror.Widget.TodoList;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;

import com.example.oh_sungjin.prjarachne_smartmirror.R;

public class MyTodoList extends LinearLayout {

    private TextView dateText;
    private TextView timeText;
    private TextView titleText;
    private TextView contentsText;

    private TodoListViewAdapter todoListViewAdapter;

    public MyTodoList(Context context){
        this(context, null);
    }

    public MyTodoList(Context context, AttributeSet attrs){
        this(context, attrs, 0);
    }

    public MyTodoList(Context context, AttributeSet attrs, int defStyleAttr){
        super(context, attrs, defStyleAttr);

        getAttrs(attrs, defStyleAttr);

        String infService = Context.LAYOUT_INFLATER_SERVICE;
        LayoutInflater li = (LayoutInflater) getContext().getSystemService(infService);
        View v = li.inflate(R.layout.todolist, this, false);
        addView(v);

        ListView todoListView;

        todoListViewAdapter = new TodoListViewAdapter();

        todoListView = (ListView)findViewById(R.id.todolistview);
        todoListView.setAdapter(todoListViewAdapter);

        //test 데이터
        //todoListViewAdapter.addItem("2018-03-01", "08:00 AM", "Test Title 1", "Test Contents 1");

    }

    private void getAttrs(AttributeSet attrs){
        TypedArray typedArray = getContext().obtainStyledAttributes(attrs, R.styleable.TodoListAttrs);

        setTypeArray(typedArray);
    }

    private void getAttrs(AttributeSet attrs, int defStyle){
        TypedArray typedArray = getContext().obtainStyledAttributes(attrs, R.styleable.TodoListAttrs, defStyle ,0);

        setTypeArray(typedArray);
    }

    private void setTypeArray(TypedArray typedArray){
        int dateColor = typedArray.getColor(R.styleable.TodoListAttrs_dateColor, 0);
        int timeColor = typedArray.getColor(R.styleable.TodoListAttrs_timeColor, 0);
        int titleColor = typedArray.getColor(R.styleable.TodoListAttrs_titleColor, 0);
        int contentsColor = typedArray.getColor(R.styleable.TodoListAttrs_contentsColor, 0);

        typedArray.recycle();
    }

    public void setTodoListData(String date, String time, String title, String contents){
        todoListViewAdapter.addItem(date, time, title, contents);
    }

}
