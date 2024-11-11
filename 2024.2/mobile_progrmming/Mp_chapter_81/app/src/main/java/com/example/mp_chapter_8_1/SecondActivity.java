package com.example.mp_chapter_8_1;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RatingBar;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class SecondActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle saveInstanceState){
        super.onCreate(saveInstanceState);
        setContentView(R.layout.seond_activity);  // 'second_activity'로 파일 이름 확인

        Intent intent = getIntent();
        int[] voteResult = intent.getIntArrayExtra("VoteCount");  // int 배열로 받기
        String[] imageName = intent.getStringArrayExtra("ImageName");  // String 배열로 받기
        int best_idx = 0;

        RatingBar rbar[] = new RatingBar[9];
        TextView tv[] = new TextView[9];

        Integer rbarID[] = {R.id.a, R.id.b, R.id.c, R.id.d, R.id.e, R.id.f, R.id.g, R.id.h, R.id.i};

        for (int i = 0; i < voteResult.length; i++) {
            rbar[i] = (RatingBar) findViewById(rbarID[i]);
            rbar[i].setRating((float) voteResult[i]);
            if (voteResult[best_idx] < voteResult[i]) {
                best_idx = i;
            }
        }

        TextView best_name = (TextView) findViewById(R.id.best_name);
        ImageView best_image = (ImageView) findViewById(R.id.best_image);

        best_name.setText(imageName[best_idx]);

        Button butReturn = (Button) findViewById(R.id.butReturn);
        butReturn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });
    }
}
