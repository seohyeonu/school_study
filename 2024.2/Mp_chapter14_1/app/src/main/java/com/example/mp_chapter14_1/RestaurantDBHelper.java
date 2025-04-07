package com.example.mp_chapter14_1;


import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class RestaurantDBHelper extends SQLiteOpenHelper {

    private static final String DATABASE_NAME = "restaurants.db";
    private static final int DATABASE_VERSION = 1;

    public RestaurantDBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String createTable = "CREATE TABLE restaurants (" +
                "id INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "name TEXT, " +
                "latitude REAL, " +
                "longitude REAL)";
        db.execSQL(createTable);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS restaurants");
        onCreate(db);
    }

    public void insertRestaurant(String name, double latitude, double longitude) {
        SQLiteDatabase db = this.getWritableDatabase();
        String insertQuery = "INSERT INTO restaurants (name, latitude, longitude) VALUES ('" + name + "', " + latitude + ", " + longitude + ")";
        db.execSQL(insertQuery);
    }
}
