package com.example.oh_sungjin.prjarachne_smartmirror.Widget.TodoList;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.example.oh_sungjin.prjarachne_smartmirror.R;

import java.util.ArrayList;

public class TodoListViewAdapter extends BaseAdapter{

    private ArrayList<TodoListItem> todoListViewItemList = new ArrayList<TodoListItem>();

    public TodoListViewAdapter(){
        //생성자
    }

    @Override
    public int getCount() {
        return todoListViewItemList.size();
    }

    @Override
    public View getView(int position, View view, ViewGroup viewGroup) {
        final int pos = position;
        final Context context = viewGroup.getContext();

        if(view == null){
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = inflater.inflate(R.layout.todolist_item, viewGroup, false);
        }

        TextView dateTextView = (TextView)view.findViewById(R.id.todoDate);
        TextView timeTextView = (TextView)view.findViewById(R.id.todoTime);
        TextView titleTextView = (TextView)view.findViewById(R.id.todoTitle);
        TextView contentsTextView = (TextView)view.findViewById(R.id.todoContents);

        TodoListItem todoListItem = todoListViewItemList.get(position);

        dateTextView.setText(todoListItem.getDate());
        timeTextView.setText(todoListItem.getTime());
        titleTextView.setText(todoListItem.getTitle());
        contentsTextView.setText(todoListItem.getContents());

        return view;

    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public Object getItem(int position) {
        return todoListViewItemList.get(position);
    }

    public void addItem(String date, String time, String title, String contents){
        TodoListItem item = new TodoListItem();

        item.setDate(date);
        item.setTime(time);
        item.setTitle(title);
        item.setContents(contents);

        todoListViewItemList.add(item);
    }

}
