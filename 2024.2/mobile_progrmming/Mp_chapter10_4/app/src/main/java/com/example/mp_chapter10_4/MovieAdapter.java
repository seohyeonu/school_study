package com.example.mp_chapter10_4;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

public class MovieAdapter extends RecyclerView.Adapter<MovieAdapter.ViewHolder> {
    private final List<Movies> movieList;
    private OnItemClickListener onItemClickListener;

    public MovieAdapter(List<Movies> movieList) {
        this.movieList = movieList;
    }

    public interface OnItemClickListener {
        void onItemClick(int position, Movies movie);
    }

    public void setOnItemClickListener(OnItemClickListener listener) {
        this.onItemClickListener = listener;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.item_movie, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        // position을 원본 데이터 크기 내로 변환
        int actualPosition = position % movieList.size();
        Movies movie = movieList.get(actualPosition);

        holder.thumbnail.setImageResource(movie.getImageResId());
        holder.itemView.setOnClickListener(v -> {
            if (onItemClickListener != null) {
                onItemClickListener.onItemClick(actualPosition, movie);
            }
        });
    }

    @Override
    public int getItemCount() {
        // 거의 무한한 아이템 수를 반환
        return Integer.MAX_VALUE;
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public ImageView thumbnail;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            thumbnail = itemView.findViewById(R.id.thumbnail);
        }
    }
}
