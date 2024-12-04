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

public class FindUserPasswordActivity extends AppCompatActivity {
    private int is_changed_id = -1;
    private int is_ready_forwarding = -1;
    private int is_random_checkNum = -9999990;
    private int is_final_checkStatus = -1;
    ImageView appearedCheckIcon, appearedPasswordIcon;
    ImageButton backButton, forwardingButton, appearedCheckButton;
    Button newUserPasswordButton;
    EditText inputID, inputPhoneNum, appearedInputCheckNum, appearedInputPassword;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.find_user_password_activity);
        //
        //선언해줘야 할 위젯들
        backButton = findViewById(R.id.backButton); // (완료)
        forwardingButton = findViewById(R.id.forwardingButton); // (완료)
        newUserPasswordButton = findViewById(R.id.new_user_password); // (미완료)
        inputID = findViewById(R.id.input_id);  // (완료)
        inputPhoneNum = findViewById(R.id.input_phoneNum);

        // 인증번호 전송 후에 보여야 할 인증번호 기입 칸을 위해 위젯 선언 해주기
        appearedCheckIcon = findViewById(R.id.checkbox_icon);
        appearedInputCheckNum = findViewById(R.id.input_checkNum);
        appearedCheckButton = findViewById(R.id.checkButton);

        // 인증 완료가 되면 비밀번호 재설정을 위해 기입 칸이 보이도록하기 위해 위젯 선언 해주기
        appearedPasswordIcon = findViewById(R.id.password_icon);
        appearedInputPassword = findViewById(R.id.input_password);

        // 입력에서 무결성을 확인 해주기 위한 작업들 입니다.
        inputID.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                is_changed_id = -1;
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
        forwardingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputID.getText().toString().length() == 0){
                    Toast.makeText(getApplicationContext(), "아이디를 입력해주세요.", Toast.LENGTH_SHORT).show();
                }
                else if (is_ready_forwarding == -1) {
                    Toast.makeText(getApplicationContext(), "전화번호 입력이 틀렸습니다.", Toast.LENGTH_SHORT).show();
                }
                else {
                    Random randomNum = new Random();
                    is_random_checkNum = randomNum.nextInt(10) * 100000 + randomNum.nextInt(10) * 10000 + randomNum.nextInt(10) * 1000 +
                            randomNum.nextInt(10) * 100 + randomNum.nextInt(10) * 10 + randomNum.nextInt(10);
                    Toast.makeText(getApplicationContext(), String.valueOf(is_random_checkNum), Toast.LENGTH_LONG).show();

                    appearedCheckIcon.setVisibility(View.VISIBLE);
                    appearedInputCheckNum.setVisibility(View.VISIBLE);
                    appearedCheckButton.setVisibility(View.VISIBLE);
                }
                appearedInputCheckNum.setText("");
            }
        });
        // 아이디 + 전화번호를 대조해서 해당 유저가 있는지 탐색하는 구조도 추가해줘야 함 (미완ㅣ)
        appearedCheckButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputID.getText().toString().length() == 0){
                    Toast.makeText(getApplicationContext(), "아이디를 입력해주세요.", Toast.LENGTH_SHORT).show();
                }
                else if (is_ready_forwarding == -1) {
                    Toast.makeText(getApplicationContext(), "전화번호 입력이 틀렸습니다.", Toast.LENGTH_SHORT).show();
                }
                else if(appearedInputCheckNum.getText().toString().equals(String.valueOf(is_random_checkNum)) == true) {
                    Toast.makeText(getApplicationContext(), "인증이 완료 됐습니다.", Toast.LENGTH_SHORT).show();
                    is_changed_id = 0;
                    is_final_checkStatus = 0;

                    inputID.setFocusableInTouchMode(false);
                    inputPhoneNum.setFocusableInTouchMode(false);
                    appearedInputCheckNum.setFocusableInTouchMode(false);
                    forwardingButton.setClickable(false);
                    appearedCheckButton.setClickable(false);

                    appearedPasswordIcon.setVisibility(View.VISIBLE);
                    appearedInputPassword.setVisibility(View.VISIBLE);
                }
                else {
                    Toast.makeText(getApplicationContext(), "인증번호 입력이 틀렸습니다.", Toast.LENGTH_SHORT).show();
                }
            }
        });
        // 새로운 비밀번호 적용해주는 작업을 해줘야 함 (미완)
        newUserPasswordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(appearedInputPassword.getText().toString().length() != 0){
                    Toast.makeText(getApplicationContext(), "비밀번호가 변경 됐습니다.", Toast.LENGTH_SHORT).show();
                    finish();
                }
                else {
                    Toast.makeText(getApplicationContext(), "새로 등록한 비밀번호를 입력해주세요.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        //
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.find_user_password_page), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
}