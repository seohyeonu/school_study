package com.example.mp_chapter_8_1;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        Button vote_finish = (Button) findViewById(R.id.vote);
        final int voteCount[] = new int[9];
        for(int i=0; i<9; i++){
            voteCount[i] = 0;
        }

        ImageView image[] = new ImageView[9];
        Integer imageId[] = {R.id.a, R.id.b ,R.id.c,R.id.d,R.id.e,R.id.f,R.id.g,R.id.h,R.id.i};
        final  String imaName[] = {"물범", "북극곰", "북극여우", "앵무새", "호랑이", "래서판다", "낙타", "사막여우", "미어캣"};

        for(int i=0; i<imageId.length; i++){
            final  int index;
            index = i;
            image[index] = (ImageView) findViewById(imageId[index]);
            image[index].setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    voteCount[index]++;
                    Toast.makeText(getApplicationContext(),imaName[index] +  " : 총" + voteCount[index] + "표",
                    Toast.LENGTH_SHORT).show();
                }
            });
        }

        vote_finish.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), SecondActivity.class);
                intent.putExtra("VoteCount", voteCount);
                intent.putExtra("ImageName", imaName);
                startActivity(intent);
            }
        });
    }
}