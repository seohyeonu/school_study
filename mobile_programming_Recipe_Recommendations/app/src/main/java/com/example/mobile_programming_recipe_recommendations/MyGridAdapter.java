package com.example.mobile_programming_recipe_recommendations;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridLayout;
import android.widget.GridView;
import android.widget.ImageView;

public class MyGridAdapter  extends BaseAdapter {
    Context context;

    Integer[] buttonID = {
            R.drawable.button_side, R.drawable.button_noodle,R.drawable.button_soup,
            R.drawable.button_snack,R.drawable.button_heart,R.drawable.button_main,
            R.drawable.button_salad,R.drawable.button_night,R.drawable.button_else,
            R.drawable.button_side, R.drawable.button_noodle,R.drawable.button_soup,
            R.drawable.button_snack,R.drawable.button_heart,R.drawable.button_main,
            R.drawable.button_salad,R.drawable.button_night,R.drawable.button_else,
            R.drawable.button_side, R.drawable.button_noodle,R.drawable.button_soup,
            R.drawable.button_snack,R.drawable.button_heart,R.drawable.button_main,
            R.drawable.button_salad,R.drawable.button_night,R.drawable.button_else
    };

    public MyGridAdapter(Context c){
        context = c;
    }

    @Override
    public int getCount() {
        return buttonID.length;
    }

    @Override
    public Object getItem(int i) {
        return null;
    }

    @Override
    public long getItemId(int i) {
        return 0;
    }

    @Override
    public View getView(int position, View view, ViewGroup parent) {
        ImageView imageView = new ImageView(context);
        imageView.setLayoutParams(new GridView.LayoutParams(200, 300));
        imageView.setScaleType(ImageView.ScaleType.FIT_CENTER);
        imageView.setPadding(5, 5, 5, 5);

        imageView.setImageResource(buttonID[position]);

        return imageView;
    }
}