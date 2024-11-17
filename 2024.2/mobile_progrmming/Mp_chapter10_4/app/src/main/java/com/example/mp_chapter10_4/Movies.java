package com.example.mp_chapter10_4;

public class Movies {
    private String title;
    private int imageResId;
    private String director;

    public Movies(String title, int imageResId, String director) {
        this.title = title;
        this.imageResId = imageResId;
        this.director = director;
    }

    public String getTitle() {
        return title;
    }

    public int getImageResId() {
        return imageResId;
    }

    public String getDirector() {
        return director;
    }
}
