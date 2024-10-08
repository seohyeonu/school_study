package com.example.basic;

import android.content.Intent;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {
    Button btn1;
    Button btn2;
    Button btn3;
    Button btn4;
    Button btn5;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState); // 상속 받은 것
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main); //UI 연걸

        //xml에서 생성한 버튼이랑 연결
        btn1 =(Button) findViewById(R.id.button1);
        btn2 =(Button) findViewById(R.id.button2);
        btn3 =(Button) findViewById(R.id.button3);
//        btn4 =(Button) findViewById(R.id.button4);
//        btn5 =(Button) findViewById(R.id.button5);

        btn1.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                Toast.makeText(getApplicationContext(), "20213348-서현우", Toast.LENGTH_SHORT).show();
            }
        });

        btn2.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                finish();
            }
        });
        btn3.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                String url = "http://www.naver.com";
                Intent naver = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
                startActivity(naver);
            }
        });
        btn4.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                btn4.setBackgroundColor(Color.GRAY);
            }
        });
        btn5.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                btn5.setTextColor(Color.GRAY);
            }
        });
    }
}