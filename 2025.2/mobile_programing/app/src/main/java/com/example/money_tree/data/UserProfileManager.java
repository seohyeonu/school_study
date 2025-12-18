package com.example.money_tree.data;

import android.content.Context;
import android.content.SharedPreferences;
import java.util.Calendar;

public class UserProfileManager {
    private final SharedPreferences prefs;

    private static final String KEY_NAME = "user_name";
    private static final String KEY_MONTHLY_INCOME = "monthly_income";
    private static final String KEY_FIXED_EXPENSE = "fixed_expense";
    private static final String KEY_SAVING_GOAL = "saving_goal";
    private static final String KEY_IS_SETUP_COMPLETE = "is_setup_complete";

    public UserProfileManager(Context context) {
        prefs = context.getSharedPreferences("user_prefs", Context.MODE_PRIVATE);
    }

    public String getName() {
        return prefs.getString(KEY_NAME, null);
    }

    public void setName(String name) {
        prefs.edit().putString(KEY_NAME, name).apply();
    }

    public long getMonthlyIncome() {
        return prefs.getLong(KEY_MONTHLY_INCOME, 0);
    }

    public void setMonthlyIncome(long income) {
        prefs.edit().putLong(KEY_MONTHLY_INCOME, income).apply();
    }

    public long getFixedExpense() {
        return prefs.getLong(KEY_FIXED_EXPENSE, 0);
    }

    public void setFixedExpense(long expense) {
        prefs.edit().putLong(KEY_FIXED_EXPENSE, expense).apply();
    }

    public long getSavingGoal() {
        return prefs.getLong(KEY_SAVING_GOAL, 0);
    }

    public void setSavingGoal(long goal) {
        prefs.edit().putLong(KEY_SAVING_GOAL, goal).apply();
    }

    public boolean isSetupComplete() {
        return prefs.getBoolean(KEY_IS_SETUP_COMPLETE, false);
    }

    public void setSetupComplete(boolean isComplete) {
        prefs.edit().putBoolean(KEY_IS_SETUP_COMPLETE, isComplete).apply();
    }

    public long calculateDailyAllowance() {
        Calendar calendar = Calendar.getInstance();
        int daysInMonth = calendar.getActualMaximum(Calendar.DAY_OF_MONTH);

        long disposableIncome = getMonthlyIncome() - (getFixedExpense() + getSavingGoal());
        return disposableIncome / daysInMonth;
    }
}
