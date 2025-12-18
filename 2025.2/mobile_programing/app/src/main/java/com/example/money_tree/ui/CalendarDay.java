package com.example.money_tree.ui;

public class CalendarDay {
    private int day; // 1~31, or 0 for empty padding
    private long income;
    private long expense;
    private long dailyAllowance;
    private boolean isToday;

    public CalendarDay(int day, long income, long expense, long dailyAllowance, boolean isToday) {
        this.day = day;
        this.income = income;
        this.expense = expense;
        this.dailyAllowance = dailyAllowance;
        this.isToday = isToday;
    }

    public CalendarDay(int day) {
        this(day, 0, 0, 0, false);
    }

    public int getDay() { return day; }
    public long getIncome() { return income; }
    public long getExpense() { return expense; }
    public long getDailyAllowance() { return dailyAllowance; }
    public boolean isToday() { return isToday; }
}
