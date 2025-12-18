package com.example.money_tree.ui;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import com.example.money_tree.R;
import com.example.money_tree.data.AppDatabase;
import com.example.money_tree.data.Transaction;
import com.example.money_tree.data.UserProfileManager;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class CalendarActivity extends AppCompatActivity {

    private TextView tvMonthTitle;
    private ImageButton btnPrevMonth;
    private ImageButton btnNextMonth;
    private RecyclerView rvCalendar;
    private ImageButton btnNavCalendar;
    private ImageButton btnNavAdd;
    private ImageButton btnNavHome;

    private final Calendar calendar = Calendar.getInstance();
    private UserProfileManager userProfileManager;
    private final ExecutorService executorService = Executors.newSingleThreadExecutor();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_calendar);

        userProfileManager = new UserProfileManager(this);

        initViews();
        setupListeners();
        updateCalendar();
    }

    @Override
    protected void onResume() {
        super.onResume();
        updateCalendar();
    }

    private void initViews() {
        tvMonthTitle = findViewById(R.id.tv_month_title);
        btnPrevMonth = findViewById(R.id.btn_prev_month);
        btnNextMonth = findViewById(R.id.btn_next_month);
        rvCalendar = findViewById(R.id.rv_calendar);
        btnNavCalendar = findViewById(R.id.btn_nav_calendar);
        btnNavAdd = findViewById(R.id.btn_nav_add);
        btnNavHome = findViewById(R.id.btn_nav_home);

        rvCalendar.setLayoutManager(new GridLayoutManager(this, 7));
    }

    private void setupListeners() {
        btnPrevMonth.setOnClickListener(v -> {
            calendar.add(Calendar.MONTH, -1);
            updateCalendar();
        });
        btnNextMonth.setOnClickListener(v -> {
            calendar.add(Calendar.MONTH, 1);
            updateCalendar();
        });
        btnNavCalendar.setOnClickListener(v -> {
            // Already on Calendar
        });
        btnNavAdd.setOnClickListener(v -> startActivity(new Intent(this, AddTransactionActivity.class)));
        btnNavHome.setOnClickListener(v -> {
            startActivity(new Intent(this, HomeActivity.class));
            finish();
        });
    }

    private void updateCalendar() {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy년 MM월", Locale.KOREA);
        tvMonthTitle.setText(sdf.format(calendar.getTime()));

        executorService.execute(() -> {
            List<CalendarDay> days = generateCalendarDays();
            runOnUiThread(() -> {
                CalendarAdapter adapter = new CalendarAdapter(days, day -> {
                    if (day.getDay() != 0) {
                        Intent intent = new Intent(CalendarActivity.this, DetailActivity.class);
                        Calendar selectedDate = Calendar.getInstance();
                        selectedDate.set(calendar.get(Calendar.YEAR), calendar.get(Calendar.MONTH), day.getDay());
                        intent.putExtra("selected_date", selectedDate.getTimeInMillis());
                        startActivity(intent);
                    }
                });
                rvCalendar.setAdapter(adapter);
            });
        });
    }

    private List<CalendarDay> generateCalendarDays() {
        int year = calendar.get(Calendar.YEAR);
        int month = calendar.get(Calendar.MONTH);

        Calendar tempCal = Calendar.getInstance();
        tempCal.set(year, month, 1, 0, 0, 0);
        tempCal.set(Calendar.MILLISECOND, 0);

        int daysInMonth = tempCal.getActualMaximum(Calendar.DAY_OF_MONTH);
        int startDayOfWeek = tempCal.get(Calendar.DAY_OF_WEEK); // 1=Sunday

        long disposable = userProfileManager.getMonthlyIncome() - (userProfileManager.getFixedExpense() + userProfileManager.getSavingGoal());
        long dailyAllowance = disposable / daysInMonth;

        long startOfMonth = tempCal.getTimeInMillis();
        tempCal.set(Calendar.DAY_OF_MONTH, daysInMonth);
        tempCal.set(Calendar.HOUR_OF_DAY, 23);
        tempCal.set(Calendar.MINUTE, 59);
        tempCal.set(Calendar.SECOND, 59);
        long endOfMonth = tempCal.getTimeInMillis();

        AppDatabase db = AppDatabase.getDatabase(this);
        List<Transaction> transactions = db.transactionDao().getByDateRange(startOfMonth, endOfMonth);

        List<CalendarDay> days = new ArrayList<>();

        for (int i = 1; i < startDayOfWeek; i++) {
            days.add(new CalendarDay(0));
        }

        Calendar todayCal = Calendar.getInstance();
        boolean isCurrentMonth = todayCal.get(Calendar.YEAR) == year && todayCal.get(Calendar.MONTH) == month;
        int todayDay = todayCal.get(Calendar.DAY_OF_MONTH);

        for (int day = 1; day <= daysInMonth; day++) {
            List<Transaction> dayTransactions = new ArrayList<>();
            for (Transaction t : transactions) {
                Calendar tCal = Calendar.getInstance();
                tCal.setTimeInMillis(t.getDate());
                if (tCal.get(Calendar.DAY_OF_MONTH) == day) {
                    dayTransactions.add(t);
                }
            }

            long incomeSum = 0;
            long expenseSum = 0;
            for (Transaction t : dayTransactions) {
                if ("INCOME".equals(t.getType())) incomeSum += t.getAmount();
                if ("EXPENSE".equals(t.getType())) expenseSum += t.getAmount();
            }

            days.add(new CalendarDay(
                    day,
                    incomeSum,
                    expenseSum,
                    dailyAllowance,
                    isCurrentMonth && day == todayDay
            ));
        }

        return days;
    }
}
