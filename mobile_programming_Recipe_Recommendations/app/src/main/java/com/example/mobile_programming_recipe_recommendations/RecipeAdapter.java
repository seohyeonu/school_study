package com.example.mobile_programming_recipe_recommendations;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;

public class RecipeAdapter extends BaseAdapter {
    private Context context;
    private Integer[] imageIds;

    // 생성자
    public RecipeAdapter(Context context, Integer[] imageIds) {
        this.context = context;
        this.imageIds = imageIds;
    }

    @Override
    public int getCount() {
        return imageIds.length;
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            LayoutInflater inflater = LayoutInflater.from(context);
            convertView = inflater.inflate(R.layout.activity_listview, parent, false);
        }

        ImageView imageView = convertView.findViewById(R.id.list_img);

        imageView.setImageResource(imageIds[position]);

        return convertView;
    }


}