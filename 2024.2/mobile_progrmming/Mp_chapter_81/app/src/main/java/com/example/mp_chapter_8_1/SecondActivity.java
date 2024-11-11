package com.example.mp_chapter_8_1;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RatingBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class SecondActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle saveInstanceState) {
        super.onCreate(saveInstanceState);
        setContentView(R.layout.second_activity);  // 파일 이름 수정

        int best_idx = 0;

        Intent intent = getIntent();
        int[] voteResult = intent.getIntArrayExtra("VoteCount");
        String[] imageName = intent.getStringArrayExtra("ImageName");

        TextView tv[] = new TextView[imageName.length];
        RatingBar rbar[] = new RatingBar[imageName.length];

        Integer tvID[] = {R.id.tv1, R.id.tv2, R.id.tv3, R.id.tv4, R.id.tv5, R.id.tv6, R.id.tv7, R.id.tv8,R.id.tv9};
        Integer rbarID[] = {R.id.a, R.id.b, R.id.c, R.id.d, R.id.e, R.id.f, R.id.g, R.id.h, R.id.i};
        Integer imageFileId[] = {R.drawable.seal, R.drawable.polar_bear, R.drawable.arctic_fox, R.drawable.parrot, R.drawable.tiger,  R.drawable.panda, R.drawable.camel, R.drawable.fennec_fox, R.drawable.meerkat};

        for(int i=0; i<voteResult.length; i++){
            tv[i] = (TextView) findViewById(tvID[i]);
            rbar[i] = (RatingBar) findViewById(rbarID[i]);
        }

        for(int i=0; i<voteResult.length; i++){
            tv[i].setText(imageName[i]);
            rbar[i].setRating((float) voteResult[i]);

            if(voteResult[best_idx] <= voteResult[i]){
                best_idx = i;
            }
        }

        TextView textView = (TextView) findViewById(R.id.best_name);
        textView.setText(imageName[best_idx]);

        ImageView imageView = (ImageView) findViewById(R.id.best_image);

        imageView.setImageResource(imageFileId[best_idx]);

        Button btuReturn = (Button) findViewById(R.id.butReturn);
        btuReturn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });
    }
}
