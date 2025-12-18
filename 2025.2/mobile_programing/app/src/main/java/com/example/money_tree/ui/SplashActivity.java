package com.example.money_tree.ui;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import androidx.appcompat.app.AppCompatActivity;
import com.example.money_tree.R;
import com.example.money_tree.data.UserProfileManager;

public class SplashActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        new Handler(Looper.getMainLooper()).postDelayed(() -> {
            UserProfileManager userProfileManager = new UserProfileManager(this);
            if (userProfileManager.isSetupComplete()) {
                startActivity(new Intent(this, HomeActivity.class));
            } else {
                startActivity(new Intent(this, OnboardingActivity.class));
            }
            finish();
        }, 2000); // 2 seconds delay
    }
}
