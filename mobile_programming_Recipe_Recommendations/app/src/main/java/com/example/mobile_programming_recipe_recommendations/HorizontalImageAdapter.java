package com.example.mobile_programming_recipe_recommendations;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

public class HorizontalImageAdapter extends RecyclerView.Adapter<HorizontalImageAdapter.ViewHolder> {

    private int[] images;
    private OnItemClickListener listener;

    // 생성자에서 이미지 배열 전달
    public HorizontalImageAdapter(int[] images, OnItemClickListener listener) {
        this.images = images;
        this.listener = listener;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        ImageButton imageButton = new ImageButton(parent.getContext());
        imageButton.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        imageButton.setScaleType(ImageButton.ScaleType.FIT_XY);
        imageButton.setBackground(null);

        return new ViewHolder(imageButton);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        holder.imageButton.setImageResource(images[position]);
        holder.bind(position, listener);
    }

    @Override
    public int getItemCount() {
        return images.length;
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        ImageButton imageButton;

        public ViewHolder(View itemView) {
            super(itemView);
            imageButton = (ImageButton) itemView;
        }

        public void bind(final int position, final OnItemClickListener listener) {
            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    listener.onItemClick(position);
                }
            });
        }
    }

    public interface OnItemClickListener {
        void onItemClick(int position);
    }
}