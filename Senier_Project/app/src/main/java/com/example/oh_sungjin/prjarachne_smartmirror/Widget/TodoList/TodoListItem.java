package com.example.oh_sungjin.prjarachne_smartmirror.Widget.TodoList;

public class TodoListItem {
    private String date;        //yyyy-MM-dd
    private String time;        //hh:mm aa
    private String Title;
    private String contents;

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public String getTitle() {
        return Title;
    }

    public void setTitle(String title) {
        Title = title;
    }

    public String getContents() {
        return contents;
    }

    public void setContents(String contents) {
        this.contents = contents;
    }

}
