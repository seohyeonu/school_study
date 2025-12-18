package com.example.money_tree.ui;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import com.example.money_tree.R;
import com.example.money_tree.data.AppDatabase;
import com.example.money_tree.data.Transaction;
import com.example.money_tree.data.UserProfileManager;
import java.text.NumberFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class DetailActivity extends AppCompatActivity {

    private TextView tvDetailDate;
    private TextView tvDailySummaryMessage;
    private TextView tvDailyAmountInfo;
    private RecyclerView rvTransactions;
    private ImageButton btnNavCalendar;
    private ImageButton btnNavAdd;
    private ImageButton btnNavHome;

    private long selectedDateMillis;
    private UserProfileManager userProfileManager;
    private final ExecutorService executorService = Executors.newSingleThreadExecutor();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_daily_detail);

        userProfileManager = new UserProfileManager(this);
        selectedDateMillis = getIntent().getLongExtra("selected_date", System.currentTimeMillis());

        initViews();
        setupListeners();
        loadData();
    }

    private void initViews() {
        tvDetailDate = findViewById(R.id.tv_detail_date);
        tvDailySummaryMessage = findViewById(R.id.tv_daily_summary_message);
        tvDailyAmountInfo = findViewById(R.id.tv_daily_amount_info);
        rvTransactions = findViewById(R.id.rv_transactions);
        btnNavCalendar = findViewById(R.id.btn_nav_calendar);
        btnNavAdd = findViewById(R.id.btn_nav_add);
        btnNavHome = findViewById(R.id.btn_nav_home);

        rvTransactions.setLayoutManager(new LinearLayoutManager(this));
    }

    private void setupListeners() {
        btnNavCalendar.setOnClickListener(v -> {
            startActivity(new Intent(this, CalendarActivity.class));
            finish();
        });
        btnNavAdd.setOnClickListener(v -> startActivity(new Intent(this, AddTransactionActivity.class)));
        btnNavHome.setOnClickListener(v -> {
            startActivity(new Intent(this, HomeActivity.class));
            finish();
        });
    }

    private void loadData() {
        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(selectedDateMillis);

        SimpleDateFormat sdf = new SimpleDateFormat("yyyy년 MM월 dd일", Locale.KOREA);
        tvDetailDate.setText(sdf.format(calendar.getTime()));

        int daysInMonth = calendar.getActualMaximum(Calendar.DAY_OF_MONTH);
        long disposable = userProfileManager.getMonthlyIncome() - (userProfileManager.getFixedExpense() + userProfileManager.getSavingGoal());
        long dailyAllowance = disposable / daysInMonth;

        executorService.execute(() -> {
            calendar.set(Calendar.HOUR_OF_DAY, 0);
            calendar.set(Calendar.MINUTE, 0);
            calendar.set(Calendar.SECOND, 0);
            calendar.set(Calendar.MILLISECOND, 0);
            long startOfDay = calendar.getTimeInMillis();

            long endOfDay = startOfDay + 24 * 60 * 60 * 1000 - 1;

            AppDatabase db = AppDatabase.getDatabase(this);
            List<Transaction> transactions = db.transactionDao().getByDateRange(startOfDay, endOfDay);

            long todaySpent = 0;
            for (Transaction t : transactions) {
                if ("EXPENSE".equals(t.getType())) {
                    todaySpent += t.getAmount();
                }
            }

            long finalTodaySpent = todaySpent;
            runOnUiThread(() -> {
                NumberFormat currencyFormat = NumberFormat.getNumberInstance(Locale.KOREA);
                tvDailyAmountInfo.setText("사용한 금액 | " + currencyFormat.format(finalTodaySpent) + "원");

                if (finalTodaySpent <= dailyAllowance) {
                    long saved = dailyAllowance - finalTodaySpent;
                    tvDailySummaryMessage.setText("이달 목표 하루 " + currencyFormat.format(dailyAllowance) + "원\n" + currencyFormat.format(saved) + "원 절약 성공!");
                } else {
                    long over = finalTodaySpent - dailyAllowance;
                    tvDailySummaryMessage.setText("이달 목표 하루 " + currencyFormat.format(dailyAllowance) + "원\n" + currencyFormat.format(over) + "원 초과 지출!");
                }

                rvTransactions.setAdapter(new TransactionAdapter(transactions));
            });
        });
    }
}
