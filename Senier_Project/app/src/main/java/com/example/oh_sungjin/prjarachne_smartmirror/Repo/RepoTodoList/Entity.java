package com.example.oh_sungjin.prjarachne_smartmirror.Repo.RepoTodoList;

import com.google.gson.annotations.SerializedName;

public class Entity {

    @SerializedName("id")
    private String id;

    @SerializedName("date")
    private String date;

    @SerializedName("title")
    private String title;

    @SerializedName("contents")
    private String contents;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getContents() {
        return contents;
    }

    public void setContents(String contents) {
        this.contents = contents;
    }
}
