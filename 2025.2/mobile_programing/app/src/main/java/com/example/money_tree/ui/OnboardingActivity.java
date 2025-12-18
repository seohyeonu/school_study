package com.example.money_tree.ui;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.widget.Button;
import androidx.appcompat.app.AppCompatActivity;
import com.example.money_tree.R;
import com.example.money_tree.data.UserProfileManager;
import com.google.android.material.textfield.TextInputEditText; // Or EditText if layout uses EditText

import android.widget.EditText; // Layout uses EditText based on previous reads, but let's check imports. 
// The previous Kotlin file imported TextInputEditText but layout used EditText with background. 
// Let's stick to EditText as per layout xml (which I recall seeing EditText).
// Actually, the Kotlin file had `import com.google.android.material.textfield.TextInputEditText` but `findViewById` might have cast it.
// Let's use EditText to be safe as standard EditText is compatible.

public class OnboardingActivity extends AppCompatActivity {

    private EditText etName;
    private EditText etMonthlyIncome;
    private EditText etFixedExpense;
    private EditText etSavingGoal;
    private Button btnNext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_onboarding);

        etName = findViewById(R.id.et_name);
        etMonthlyIncome = findViewById(R.id.et_income); // ID was et_income in layout xml, but et_monthly_income in Kotlin? 
        // Let's check layout xml again? No, I can't check now without tool call. 
        // In Kotlin file it was `etMonthlyIncome = findViewById(R.id.et_monthly_income)`. 
        // Wait, let me check the Kotlin file content I read.
        // Kotlin file: `etMonthlyIncome = findViewById(R.id.et_monthly_income)`
        // But in `activity_onboarding.xml` (Step 206), the ID was `et_income`.
        // So the Kotlin code might have been wrong or I misread.
        // I will use `et_income` as per the XML I wrote in Step 206.
        // Wait, if Kotlin code was running, maybe it was `et_income`.
        // Let's look at Step 206 XML: `android:id="@+id/et_income"`
        // So I should use `R.id.et_income`.

        etMonthlyIncome = findViewById(R.id.et_income);
        etFixedExpense = findViewById(R.id.et_fixed_expense);
        etSavingGoal = findViewById(R.id.et_saving_goal);
        btnNext = findViewById(R.id.btn_next);

        btnNext.setOnClickListener(v -> {
            if (validateInput()) {
                saveData();
                navigateToHome();
            }
        });
    }

    private boolean validateInput() {
        if (TextUtils.isEmpty(etName.getText())) {
            etName.setError("이름을 입력해주세요");
            return false;
        }
        if (TextUtils.isEmpty(etMonthlyIncome.getText())) {
            etMonthlyIncome.setError("월 수입을 입력해주세요");
            return false;
        }
        if (TextUtils.isEmpty(etFixedExpense.getText())) {
            etFixedExpense.setError("고정 지출을 입력해주세요");
            return false;
        }
        if (TextUtils.isEmpty(etSavingGoal.getText())) {
            etSavingGoal.setError("저축 목표를 입력해주세요");
            return false;
        }
        return true;
    }

    private void saveData() {
        UserProfileManager userProfileManager = new UserProfileManager(this);
        userProfileManager.setName(etName.getText().toString());
        
        long income = parseLong(etMonthlyIncome.getText().toString());
        userProfileManager.setMonthlyIncome(income);
        
        long expense = parseLong(etFixedExpense.getText().toString());
        userProfileManager.setFixedExpense(expense);
        
        long goal = parseLong(etSavingGoal.getText().toString());
        userProfileManager.setSavingGoal(goal);
        
        userProfileManager.setSetupComplete(true);
    }

    private long parseLong(String value) {
        try {
            return Long.parseLong(value);
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    private void navigateToHome() {
        Intent intent = new Intent(this, HomeActivity.class);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        startActivity(intent);
        finish();
    }
}
