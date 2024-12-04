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

public class JoinTheMembershipActivity extends AppCompatActivity {
    private int is_duplicate_check = -1;
    private int is_ready_forwarding = -1;
    private int is_random_checkNum = -9999990;
    private int is_final_checkStatus = -1;
    ImageView appearedIcon;
    ImageButton backButton, duplicate_checkButton, forwardingButton, appearedCheckButton;
    Button create_an_accountButton;
    EditText inputID, inputPassword, inputPhoneNum, appearedInputCheckNum;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.join_the_membership_activity);
        //
        //선언해줘야 할 위젯들
        backButton = findViewById(R.id.backButton); // (완료)
        duplicate_checkButton = findViewById(R.id.duplicate_check); // (완료)
        forwardingButton = findViewById(R.id.forwardingButton); // (완료)
        create_an_accountButton = findViewById(R.id.create_an_account); // (미완료)
        inputID = findViewById(R.id.input_id);  // (완료)
        inputPassword = findViewById(R.id.input_password);  // (완료)
        inputPhoneNum = findViewById(R.id.input_phoneNum);  // (완료)

        // 인증번호 전송 후에 보여야 할 인증번호 기입 칸을 위해 위젯 선언 해주기
        appearedIcon = findViewById(R.id.checkbox_icon);
        appearedInputCheckNum = findViewById(R.id.input_checkNum);
        appearedCheckButton = findViewById(R.id.checkButton);

        backButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });

        // 입력에 대한 무결성을 입증하기 위한 작업들 입니다.
        inputID.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if(inputID.getText().toString().length() != 0){
                    duplicate_checkButton.setVisibility(View.VISIBLE);
                }
                else {
                    duplicate_checkButton.setVisibility(View.GONE);
                }
                appearedInputCheckNum.setText("");
            }

            @Override
            public void afterTextChanged(Editable editable) {
                is_duplicate_check = -1;
            }
        });
        inputPassword.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                appearedInputCheckNum.setText("");
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
                appearedInputCheckNum.setText("");
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

        // 버튼을 눌렀을 때 무결성이 깨졌는지 상시 체크할 수 있도록 해야겠구나.. (완료)
        duplicate_checkButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputID.getText().toString().length() != 0){
                    Toast.makeText(getApplicationContext(), "사용해도 되는 아이디 입니다.", Toast.LENGTH_SHORT).show();
                    is_duplicate_check = 0;
                }
            }
        });
        // 메시지로 전송되는 인증번호는 Toast 메시지로 대체한 프로토타입으로 제작했습니다.
        forwardingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputPassword.getText().toString().length() != 0 && is_duplicate_check == 0 && is_ready_forwarding == 0){
                    Random randomNum = new Random();
                    is_random_checkNum = randomNum.nextInt(10) * 100000 + randomNum.nextInt(10) * 10000 + randomNum.nextInt(10) * 1000 +
                            randomNum.nextInt(10) * 100 + randomNum.nextInt(10) * 10 + randomNum.nextInt(10);
                    Toast.makeText(getApplicationContext(), String.valueOf(is_random_checkNum), Toast.LENGTH_LONG).show();

                    appearedIcon.setVisibility(View.VISIBLE);
                    appearedInputCheckNum.setVisibility(View.VISIBLE);
                    appearedCheckButton.setVisibility(View.VISIBLE);
                }
                else{
                    if(inputID.getText().toString().length() == 0){
                        Toast.makeText(getApplicationContext(), "아이디를 입력해주세요", Toast.LENGTH_SHORT).show();
                    }
                    else if(is_duplicate_check == -1){
                        Toast.makeText(getApplicationContext(), "아이디 중복확인을 눌러주세요", Toast.LENGTH_SHORT).show();
                    }
                    else if(inputPassword.getText().toString().length() == 0){
                        Toast.makeText(getApplicationContext(), "비밀번호를 입력해주세요.", Toast.LENGTH_SHORT).show();
                    }
                    else if(is_ready_forwarding == -1){
                        Toast.makeText(getApplicationContext(), "전화번호 입력이 잘못됐습니다.", Toast.LENGTH_SHORT).show();
                    }
                }
                is_final_checkStatus = -1;
                appearedInputCheckNum.setText("");
            }
        });
        appearedCheckButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputPassword.getText().toString().length() != 0 && is_duplicate_check == 0 && is_ready_forwarding == 0){
                    if(appearedInputCheckNum.getText().toString().equals(String.valueOf(is_random_checkNum)) == true){
                        Toast.makeText(getApplicationContext(), "인증번호가 확인 되었습니다.", Toast.LENGTH_SHORT).show();
                        is_final_checkStatus = 0;
                    }
                    else {
                        Toast.makeText(getApplicationContext(), "인증번호가 틀렸습니다.", Toast.LENGTH_SHORT).show();
                    }
                }
                else{
                    if(inputID.getText().toString().length() == 0){
                        Toast.makeText(getApplicationContext(), "아이디를 입력해주세요", Toast.LENGTH_SHORT).show();
                    }
                    else if(is_duplicate_check == -1){
                        Toast.makeText(getApplicationContext(), "아이디 중복확인을 눌러주세요", Toast.LENGTH_SHORT).show();
                    }
                    else if(inputPassword.getText().toString().length() == 0){
                        Toast.makeText(getApplicationContext(), "비밀번호를 입력해주세요.", Toast.LENGTH_SHORT).show();
                    }
                    else if(is_ready_forwarding == -1){
                        Toast.makeText(getApplicationContext(), "전화번호 입력이 잘못됐습니다.", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
        /*
        계정이 만들어지면서 생긴 유저 정보 List를 로그인의 메인화면으로 넘겨주는 작업을 해줘야 함. (미완)
            + 로그인 메인화면에서 startActivityforResult()로 넘어와서 넘겨주면 되겠다.
        */
        create_an_accountButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(is_final_checkStatus == 0 && inputPassword.getText().toString().length() != 0 && is_duplicate_check == 0 && is_ready_forwarding == 0){
                    finish();
                }
                else{
                    if(inputID.getText().toString().length() == 0){
                        Toast.makeText(getApplicationContext(), "아이디를 입력해주세요", Toast.LENGTH_SHORT).show();
                    }
                    else if(is_duplicate_check == -1){
                        Toast.makeText(getApplicationContext(), "아이디 중복확인을 눌러주세요", Toast.LENGTH_SHORT).show();
                    }
                    else if(inputPassword.getText().toString().length() == 0){
                        Toast.makeText(getApplicationContext(), "비밀번호를 입력해주세요.", Toast.LENGTH_SHORT).show();
                    }
                    else if(is_ready_forwarding == -1){
                        Toast.makeText(getApplicationContext(), "전화번호 입력이 잘못됐습니다.", Toast.LENGTH_SHORT).show();
                    }
                    else if(is_final_checkStatus == -1){
                        Toast.makeText(getApplicationContext(), "인증번호 인증을 해주세요", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
        //
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.join_the_membership_page), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
}