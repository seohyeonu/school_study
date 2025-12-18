package com.example.money_tree.ui;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import com.example.money_tree.R;
import com.example.money_tree.data.AppDatabase;
import com.example.money_tree.data.Transaction;
import com.example.money_tree.data.UserProfileManager;
import java.text.NumberFormat;
import java.util.Calendar;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class HomeActivity extends AppCompatActivity {

    private UserProfileManager userProfileManager;
    private TextView tvDailyMessage;
    private TextView tvCurrentAllowance;
    private TextView tvTodaySpent;
    private ImageView ivBalanceCircleBg;
    private ImageView ivFeelIcon;
    private ImageButton btnSettings;
    private ImageButton btnNavCalendar;
    private ImageButton btnNavAdd;
    private ImageButton btnNavHome;

    private final ExecutorService executorService = Executors.newSingleThreadExecutor();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        userProfileManager = new UserProfileManager(this);

        initViews();
        setupListeners();
    }

    @Override
    protected void onResume() {
        super.onResume();
        updateUI();
    }

    private void initViews() {
        tvDailyMessage = findViewById(R.id.tv_daily_message);
        tvCurrentAllowance = findViewById(R.id.tv_current_allowance);
        tvTodaySpent = findViewById(R.id.tv_today_spent);
        ivBalanceCircleBg = findViewById(R.id.iv_balance_circle_bg);
        ivFeelIcon = findViewById(R.id.iv_feel_icon);
        btnSettings = findViewById(R.id.btn_settings);
        btnNavCalendar = findViewById(R.id.btn_nav_calendar);
        btnNavAdd = findViewById(R.id.btn_nav_add);
        btnNavHome = findViewById(R.id.btn_nav_home);
    }

    private void setupListeners() {
        btnSettings.setOnClickListener(v -> startActivity(new Intent(this, OnboardingActivity.class)));
        btnNavCalendar.setOnClickListener(v -> startActivity(new Intent(this, CalendarActivity.class)));
        btnNavAdd.setOnClickListener(v -> startActivity(new Intent(this, AddTransactionActivity.class)));
        btnNavHome.setOnClickListener(v -> {
            // Already on Home
        });
    }

    private void updateUI() {
        long dailyAllowance = userProfileManager.calculateDailyAllowance();
        String userName = userProfileManager.getName();

        NumberFormat currencyFormat = NumberFormat.getNumberInstance(Locale.KOREA);
        tvDailyMessage.setText(userName + "님은 " + currencyFormat.format(dailyAllowance) + "원의 행복을 실천중입니다.");

        // Background task for DB
        executorService.execute(() -> {
            long todaySpent = getTodaySpent();
            long currentAllowance = dailyAllowance - todaySpent;

            runOnUiThread(() -> {
                tvCurrentAllowance.setText(currencyFormat.format(currentAllowance) + "원");
                tvTodaySpent.setText("사용한 금액 | " + currencyFormat.format(todaySpent) + "원");
                updateStatus(currentAllowance, dailyAllowance);
            });
        });
    }

    private long getTodaySpent() {
        Calendar calendar = Calendar.getInstance();
        // Reset time to start of day
        calendar.set(Calendar.HOUR_OF_DAY, 0);
        calendar.set(Calendar.MINUTE, 0);
        calendar.set(Calendar.SECOND, 0);
        calendar.set(Calendar.MILLISECOND, 0);
        long startOfDay = calendar.getTimeInMillis();
        
        long endOfDay = startOfDay + 24 * 60 * 60 * 1000 - 1;
        
        AppDatabase db = AppDatabase.getDatabase(this);
        List<Transaction> transactions = db.transactionDao().getByDateRange(startOfDay, endOfDay);
        
        long sum = 0;
        for (Transaction t : transactions) {
            if ("EXPENSE".equals(t.getType())) {
                sum += t.getAmount();
            }
        }
        return sum;
    }

    private void updateStatus(long currentAllowance, long dailyAllowance) {
        double ratio = (dailyAllowance > 0) ? (double) currentAllowance / dailyAllowance : 0.0;

        if (ratio >= 0.8) {
            ivFeelIcon.setImageResource(android.R.drawable.star_big_on);
        } else if (ratio >= 0.3) {
            ivFeelIcon.setImageResource(android.R.drawable.star_on);
        } else if (ratio >= 0.0) {
            ivFeelIcon.setImageResource(android.R.drawable.ic_dialog_alert);
        } else {
            ivFeelIcon.setImageResource(android.R.drawable.ic_delete);
        }
    }
}
