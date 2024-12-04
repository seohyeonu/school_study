package com.example.mobile_programming_recipe_recommendations;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class BannerAdapter extends RecyclerView.Adapter<BannerAdapter.BannerViewHolder> {

    private Integer[] foodImgID;

    public BannerAdapter(Integer[] foodImgID) {
        this.foodImgID = foodImgID;
    }

    @NonNull
    @Override
    public BannerViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.activity_banner, parent, false);
        return new BannerViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull BannerViewHolder holder, int position) {
        holder.foodImageView.setImageResource(foodImgID[position]);
    }

    @Override
    public int getItemCount() {
        return foodImgID.length;
    }

    public static class BannerViewHolder extends RecyclerView.ViewHolder {
        ImageView foodImageView;

        public BannerViewHolder(@NonNull View itemView) {
            super(itemView);
            foodImageView = itemView.findViewById(R.id.foodImageView); // banner_item.xml에서 ImageView를 찾음
        }
    }
}