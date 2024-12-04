package com.example.mobile_programming_recipe_recommendations;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.util.List;

public class LoginActivity extends AppCompatActivity{
    EditText inputID, inputPassword;
    Button loginButton;
    TextView getNewUser, findUserID, findUserPass;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        //
        inputID = findViewById(R.id.input_id);
        inputPassword = findViewById(R.id.input_password);

        // 아이디와 비번이 List에 있는 확인하는 로직 + putExtra() / startActivity() 추가해주면 됨. (미완)
        loginButton = findViewById(R.id.login);
        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (inputID.getText().toString().length() != 0 && inputPassword.getText().toString().length() != 0) {
                    Toast.makeText(getApplicationContext(), inputID.getText().toString() + " 님 어서오세요!", Toast.LENGTH_SHORT).show();
                }
                else if (inputID.getText().toString().length() == 0){
                    Toast.makeText(getApplicationContext(), "아이디를 입력해주세요.", Toast.LENGTH_SHORT).show();
                }
                else if (inputPassword.getText().toString().length() == 0){
                    Toast.makeText(getApplicationContext(), "비밀번호를 입력해주세요.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        // startActivityForResult()로 이름|전화번호|아이디" List<String>을 넘겨줘야 한다. (미완)
        findUserID = findViewById(R.id.find_user_ID);
        findUserID.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent goToFindUserIDActivity = new Intent(LoginActivity.this, FindUserIDActivity.class);
                startActivity(goToFindUserIDActivity);
            }
        });

        // startActivityForResult()로 "아이디|전화번호" List<String>을 넘겨줘야 한다. (미완)
        findUserPass = findViewById(R.id.find_user_Pass);
        findUserPass.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent goToFindUserPasswordActivity = new Intent(LoginActivity.this, FindUserPasswordActivity.class);
                startActivity(goToFindUserPasswordActivity);
            }
        });

        // startActivityForResult()로 새로 만든 유저의 "이름|전화번호|아이디|비밀번호"를 받아서 List<String>에 add() 해주자. (미완)
        getNewUser = findViewById(R.id.get_new_user);
        getNewUser.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent goToJoinTheMembershipActivity = new Intent(LoginActivity.this, JoinTheMembershipActivity.class);
                //여태까지 등록된 user의 아이디가 적힌 리스트를 보내준다. 그래야 중복확인 가능하기 때문이다.
                startActivity(goToJoinTheMembershipActivity);
            }
        });
        //
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }

    ActivityResultLauncher<Intent> startForResult = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), result ->{
        Intent getData = result.getData();
        if(getData != null){
            String getValue = getData.getStringExtra("RESULT");

        }
    });
}
