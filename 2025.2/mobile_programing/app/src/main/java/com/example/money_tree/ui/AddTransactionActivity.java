package com.example.money_tree.ui;

import android.app.DatePickerDialog;
import android.content.res.ColorStateList;
import android.os.Bundle;
import android.text.TextUtils;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import com.example.money_tree.R;
import com.example.money_tree.data.AppDatabase;
import com.example.money_tree.data.Transaction;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;

public class AddTransactionActivity extends AppCompatActivity {

    private Button btnTabExpense;
    private Button btnTabIncome;
    private TextView tvDate;
    private EditText etAmount;
    private EditText etMemo;
    private Button btnSave;

    private String selectedType = "EXPENSE";
    private final Calendar calendar = Calendar.getInstance();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_transaction);

        initViews();
        setupListeners();
        updateTabUI();
        updateDateUI();
    }

    private void initViews() {
        btnTabExpense = findViewById(R.id.btn_tab_expense);
        btnTabIncome = findViewById(R.id.btn_tab_income);
        tvDate = findViewById(R.id.tv_date);
        etAmount = findViewById(R.id.et_amount);
        etMemo = findViewById(R.id.et_memo);
        btnSave = findViewById(R.id.btn_save);
    }

    private void setupListeners() {
        btnTabExpense.setOnClickListener(v -> {
            selectedType = "EXPENSE";
            updateTabUI();
        });
        btnTabIncome.setOnClickListener(v -> {
            selectedType = "INCOME";
            updateTabUI();
        });
        tvDate.setOnClickListener(v -> showDatePicker());
        btnSave.setOnClickListener(v -> saveTransaction());
    }

    private void updateTabUI() {
        if ("EXPENSE".equals(selectedType)) {
            btnTabExpense.setBackgroundTintList(ColorStateList.valueOf(ContextCompat.getColor(this, android.R.color.holo_red_light)));
            btnTabExpense.setTextColor(ContextCompat.getColor(this, android.R.color.white));
            btnTabIncome.setBackgroundTintList(ColorStateList.valueOf(ContextCompat.getColor(this, android.R.color.darker_gray)));
            btnTabIncome.setTextColor(ContextCompat.getColor(this, android.R.color.black));
        } else {
            btnTabExpense.setBackgroundTintList(ColorStateList.valueOf(ContextCompat.getColor(this, android.R.color.darker_gray)));
            btnTabExpense.setTextColor(ContextCompat.getColor(this, android.R.color.black));
            btnTabIncome.setBackgroundTintList(ColorStateList.valueOf(ContextCompat.getColor(this, android.R.color.holo_blue_light)));
            btnTabIncome.setTextColor(ContextCompat.getColor(this, android.R.color.white));
        }
    }

    private void updateDateUI() {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
        tvDate.setText(sdf.format(calendar.getTime()));
    }

    private void showDatePicker() {
        new DatePickerDialog(
                this,
                (view, year, month, dayOfMonth) -> {
                    calendar.set(year, month, dayOfMonth);
                    updateDateUI();
                },
                calendar.get(Calendar.YEAR),
                calendar.get(Calendar.MONTH),
                calendar.get(Calendar.DAY_OF_MONTH)
        ).show();
    }

    private void saveTransaction() {
        String amountStr = etAmount.getText().toString();
        if (TextUtils.isEmpty(amountStr)) {
            etAmount.setError("금액을 입력해주세요");
            return;
        }
        long amount;
        try {
            amount = Long.parseLong(amountStr);
        } catch (NumberFormatException e) {
            etAmount.setError("유효한 금액을 입력해주세요");
            return;
        }
        if (amount <= 0) {
            etAmount.setError("유효한 금액을 입력해주세요");
            return;
        }

        String memo = etMemo.getText().toString();
        long date = calendar.getTimeInMillis();

        Transaction transaction = new Transaction(
                date,
                selectedType,
                amount,
                memo,
                System.currentTimeMillis()
        );

        AppDatabase.databaseWriteExecutor.execute(() -> {
            AppDatabase.getDatabase(this).transactionDao().insert(transaction);
            runOnUiThread(() -> {
                Toast.makeText(this, "저장되었습니다.", Toast.LENGTH_SHORT).show();
                finish();
            });
        });
    }
}
