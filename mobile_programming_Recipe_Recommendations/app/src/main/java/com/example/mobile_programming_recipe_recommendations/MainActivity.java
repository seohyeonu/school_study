package com.example.mobile_programming_recipe_recommendations;

// MainActivity.java
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.PagerSnapHelper;
import androidx.recyclerview.widget.RecyclerView;

public class MainActivity extends AppCompatActivity implements HorizontalImageAdapter.OnItemClickListener {

    private String username;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 로그인 액티비티로부터 사용자 이름을 받음
        Intent intent = getIntent();
        username = intent.getStringExtra("username");

        // RecyclerView 설정
        RecyclerView recyclerView = findViewById(R.id.horizontalRecyclerView);
        LinearLayoutManager layoutManager = new LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false);
        recyclerView.setLayoutManager(layoutManager);

        // 이미지 리소스 배열 설정
        int[] images = {
                R.drawable.listpic1,
                R.drawable.piclst1,
                R.drawable.piclst2,
                R.drawable.piclst3
        };

        // Adapter 설정
        HorizontalImageAdapter adapter = new HorizontalImageAdapter(images, this);
        recyclerView.setAdapter(adapter);

        // PagerSnapHelper를 사용하여 한 번에 하나씩 스크롤하도록 설정
        PagerSnapHelper snapHelper = new PagerSnapHelper();
        snapHelper.attachToRecyclerView(recyclerView);

        // 각 버튼 클릭 리스너 설정 - 내 냉장고 액티비티로 이동 시 사용자 정보 전달
        ImageButton button1 = findViewById(R.id.button1);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, TemporaryTargetActivity.class); // 예정
                startActivity(intent);
            }
        });

        ImageButton button2 = findViewById(R.id.button2); // 내 냉장고 버튼
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, MyRefrigerator.class);
                intent.putExtra("username", username);
                startActivity(intent);
            }
        });

        ImageButton button3 = findViewById(R.id.button3);
        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, TemporaryTargetActivity.class);
                startActivity(intent);
            }
        });

        ImageButton button4 = findViewById(R.id.button4);
        button4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, TemporaryTargetActivity.class);
                startActivity(intent);
            }
        });

        ImageButton button5 = findViewById(R.id.button5);
        button5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, TemporaryTargetActivity.class);
                startActivity(intent);
            }
        });
    }

    @Override
    public void onItemClick(int position) {
        Intent intent = new Intent(MainActivity.this, TemporaryTargetActivity.class);
        startActivity(intent);
    }
}
