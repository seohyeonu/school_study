package com.example.mp_week6_widget;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Switch;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    ImageButton imageButton;
    EditText text; //첫번째 텍스트 입력
    Button text_button; // 출력 버튼
    EditText num1; // 숫자 입력
    EditText num2; // 숫자 입력 2
    Switch sw; // 스위치
    String getText;
    String getNum_string1;
    String getNum_string2;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        imageButton = findViewById(R.id.imageButton);
        text = findViewById(R.id.edit_text);
        text_button = findViewById(R.id.button);
        num1 = findViewById(R.id.num1);
        num2 = findViewById(R.id.num2);
        sw = findViewById(R.id.sw);


        imageButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(getApplicationContext(), "사진을 클릭 하였습니다.", Toast.LENGTH_SHORT).show();
            }
        });

        text_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getText = text.getText().toString();

                if(getText != null && !getText.isEmpty()){
                    Toast.makeText(getApplicationContext(), "입력하신 텍스트는 " + getText + "입니다.", Toast.LENGTH_SHORT).show();
                }else{
                    Toast.makeText(getApplicationContext(), "입력한 문자는 비어 있습니다.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        sw.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                getNum_string1=num1.getText().toString();
                getNum_string2=num2.getText().toString();

                if(isChecked){
                    if(getNum_string1 != null && getNum_string2 != null && !getNum_string1.isEmpty() && !getNum_string2.isEmpty()){
                        double result = Double.parseDouble(getNum_string1)+ Double.parseDouble(getNum_string2);
                        Toast.makeText(getApplicationContext(),"" +result , Toast.LENGTH_SHORT).show();
                    }else{
                        Toast.makeText(getApplicationContext(), "숫자를 입력해주세요.", Toast.LENGTH_SHORT).show();
                    }
                }
                else{
                    if(getNum_string1 != null && getNum_string2 != null && !getNum_string1.isEmpty() && !getNum_string2.isEmpty()){
                        double result = Double.parseDouble(getNum_string1)- Double.parseDouble(getNum_string2);
                        Toast.makeText(getApplicationContext(),"" +result , Toast.LENGTH_SHORT).show();
                    }else{
                        Toast.makeText(getApplicationContext(), "숫자를 입력해주세요.", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
    }
}