package com.example.mp_chapter10_4;

import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private RecyclerView recyclerView;
    private ImageView mainImageView;
    private TextView movieInfo;
    private LinearLayoutManager layoutManager;
    private MovieAdapter adapter;
    private List<Movies> movieList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        recyclerView = findViewById(R.id.recycler_view);
        mainImageView = findViewById(R.id.main_image);
        movieInfo = findViewById(R.id.movie_info);

        // 원본 영화 데이터 초기화
        movieList = getMovieData();

        // RecyclerView 초기화
        layoutManager = new LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false);
        recyclerView.setLayoutManager(layoutManager);
        adapter = new MovieAdapter(movieList);
        recyclerView.setAdapter(adapter);

        // RecyclerView의 초기 스크롤 위치를 중앙으로 설정
        int middlePosition = Integer.MAX_VALUE / 2;
        recyclerView.scrollToPosition(middlePosition - (middlePosition % movieList.size()));

        // 아이템 클릭 시 큰 이미지와 영화 정보 업데이트 및 중앙 정렬
        adapter.setOnItemClickListener((position, movie) -> {
            // 큰 이미지 및 정보 업데이트
            mainImageView.setImageResource(movie.getImageResId());
            movieInfo.setText("제목: " + movie.getTitle() + "\n감독: " + movie.getDirector());

            // 선택한 섬네일을 중앙에 위치
            int actualPosition = position % movieList.size();
            int offset = (recyclerView.getWidth() / 2) - getThumbnailWidth() / 2; // 중앙 오프셋 계산
            layoutManager.scrollToPositionWithOffset(position, offset);
        });
    }

    // 원본 영화 데이터 생성
    private List<Movies> getMovieData() {
        return Arrays.asList(
                new Movies("영화 1", R.drawable.seal, "감독 1"),
                new Movies("영화 2", R.drawable.panda, "감독 2"),
                new Movies("영화 3", R.drawable.polar_bear, "감독 3"),
                new Movies("영화 4", R.drawable.parrot, "감독 4"),
                new Movies("영화 5", R.drawable.meerkat, "감독 5")
        );
    }

    // 섬네일 크기 반환 (RecyclerView 아이템 크기와 동일하게 설정)
    private int getThumbnailWidth() {
        return getResources().getDimensionPixelSize(R.dimen.thumbnail_width);
    }
}
