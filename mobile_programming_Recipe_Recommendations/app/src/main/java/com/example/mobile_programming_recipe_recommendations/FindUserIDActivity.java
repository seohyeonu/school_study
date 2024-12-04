package com.example.mobile_programming_recipe_recommendations;


import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.util.Random;

public class FindUserIDActivity extends AppCompatActivity {
    private int is_changed_name = -1;
    private int is_ready_forwarding = -1;
    private int is_random_checkNum = -9999990;
    private int is_final_checkStatus = -1;
    ImageView appearedIcon;
    ImageButton backButton, forwardingButton, appearedCheckButton;
    Button find_user_idButton;
    EditText inputUserName, inputPhoneNum, appearedInputCheckNum;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.find_user_id_activity);
        //
        // 기본적으로 써야할 위젯을 위한 선언해주기
        backButton = findViewById(R.id.backButton);
        forwardingButton = findViewById(R.id.forwardingButton);
        inputUserName = findViewById(R.id.input_id);
        inputPhoneNum = findViewById(R.id.input_phoneNum);
        find_user_idButton = findViewById(R.id.find_user_id);

        // 추가적으로 보여야 할 인증번호 기입 칸을 위해 위젯 선언해주기
        appearedIcon = findViewById(R.id.checkbox_icon);
        appearedInputCheckNum = findViewById(R.id.input_checkNum);
        appearedCheckButton = findViewById(R.id.checkButton);

        // 입력 바뀔 때 무결성 검사를 해주는 작업들 입니다.
        // + 전화번호가 한자리 라도 입력이 됐을 떄 전송 버튼을 드러내기 위한 작업입니다.
        inputUserName.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                is_changed_name = -1;
                is_random_checkNum = -9999990;
                appearedInputCheckNum.setText("");
                if(is_final_checkStatus == 0) {
                    is_final_checkStatus = -2;
                }
                else {
                    is_final_checkStatus = -1;
                }
            }

            @Override
            public void afterTextChanged(Editable editable) {
            }
        });
        inputPhoneNum.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if(inputPhoneNum.getText().toString().length() != 0){
                    forwardingButton.setVisibility(View.VISIBLE);
                }
                else {
                    forwardingButton.setVisibility(View.GONE);
                }
                is_ready_forwarding = -1;
                is_random_checkNum = -9999990;
                appearedInputCheckNum.setText("");
                if(is_final_checkStatus == 0){
                    is_final_checkStatus = -2;
                }
                else {
                    is_final_checkStatus = -1;
                }
            }

            @Override
            public void afterTextChanged(Editable editable) {
                if(inputPhoneNum.getText().toString().length() == 11 && inputPhoneNum.getText().toString().matches("\\d+") == true){
                    is_ready_forwarding = 0;
                }
                else {
                    is_ready_forwarding = -1;
                }
            }
        });

        // 버튼에 대한 기능을 추가 해주기
        backButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });
        // 메시지로 전송되는 인증번호는 Toast 메시지로 대체한 프로토타입으로 제작했습니다.
        forwardingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputUserName.getText().toString().length() == 0){
                    Toast.makeText(getApplicationContext(), "이름을 입력해주세요.", Toast.LENGTH_SHORT).show();
                }
                else if (is_ready_forwarding == -1) {
                    Toast.makeText(getApplicationContext(), "전화번호 입력이 틀렸습니다.", Toast.LENGTH_SHORT).show();
                }
                else {
                    Random randomNum = new Random();
                    is_random_checkNum = randomNum.nextInt(10) * 100000 + randomNum.nextInt(10) * 10000 + randomNum.nextInt(10) * 1000 +
                            randomNum.nextInt(10) * 100 + randomNum.nextInt(10) * 10 + randomNum.nextInt(10);
                    Toast.makeText(getApplicationContext(), String.valueOf(is_random_checkNum), Toast.LENGTH_LONG).show();

                    appearedIcon.setVisibility(View.VISIBLE);
                    appearedInputCheckNum.setVisibility(View.VISIBLE);
                    appearedCheckButton.setVisibility(View.VISIBLE);
                }
                appearedInputCheckNum.setText("");
            }
        });
        appearedCheckButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputUserName.getText().toString().length() == 0){
                    Toast.makeText(getApplicationContext(), "이름을 입력해주세요.", Toast.LENGTH_SHORT).show();
                }
                else if (is_ready_forwarding == -1) {
                    Toast.makeText(getApplicationContext(), "전화번호 입력이 틀렸습니다.", Toast.LENGTH_SHORT).show();
                }
                else if(appearedInputCheckNum.getText().toString().equals(String.valueOf(is_random_checkNum)) == true) {
                    Toast.makeText(getApplicationContext(), "인증이 완료 됐습니다.", Toast.LENGTH_SHORT).show();
                    is_changed_name = 0;
                    is_final_checkStatus = 0;
                }
                else {
                    Toast.makeText(getApplicationContext(), "인증번호 입력이 틀렸습니다.", Toast.LENGTH_SHORT).show();
                }
            }
        });
        // 유저 데이터에 해당하는 유저의 이름 + 전화번호가 있는지 확인하고, 았다면 그 유저의 아이디를 출력해주자. (미완)
        find_user_idButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(is_final_checkStatus == 0 && is_changed_name == 0 && is_ready_forwarding == 0){
                    Toast.makeText(getApplicationContext(), "<유저 아이디> 출력...", Toast.LENGTH_SHORT).show();
                    finish();
                } // 이 아래는 입력이 잘못 됐거나 입력이 변경으로 인해 무결성이 깨졌을 때 경고문이 뜨게끔 만들었습니다.
                else if(inputUserName.getText().toString().length() == 0) {
                    Toast.makeText(getApplicationContext(), "이름이 입력되지 않았습니다.", Toast.LENGTH_SHORT).show();
                }
                else if(inputPhoneNum.getText().toString().length() == 0) {
                    Toast.makeText(getApplicationContext(), "전화번호가 입력되지 않았습니다.", Toast.LENGTH_SHORT).show();
                }
                else {
                    if(is_final_checkStatus == -1){
                        Toast.makeText(getApplicationContext(), "인증이 완료되지 않았습니다.", Toast.LENGTH_SHORT).show();
                    }
                    else {
                        Toast.makeText(getApplicationContext(), "입력에 변경점이 생겼습니다.", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
        //
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.find_user_id_page), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
}