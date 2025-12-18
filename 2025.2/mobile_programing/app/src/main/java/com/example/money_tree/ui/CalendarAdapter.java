package com.example.money_tree.ui;

import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import com.example.money_tree.R;
import java.text.NumberFormat;
import java.util.List;
import java.util.Locale;

public class CalendarAdapter extends RecyclerView.Adapter<CalendarAdapter.DayViewHolder> {

    private final List<CalendarDay> days;
    private final OnDayClickListener onDayClick;

    public interface OnDayClickListener {
        void onDayClick(CalendarDay day);
    }

    public CalendarAdapter(List<CalendarDay> days, OnDayClickListener onDayClick) {
        this.days = days;
        this.onDayClick = onDayClick;
    }

    @NonNull
    @Override
    public DayViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_calendar_day, parent, false);
        return new DayViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull DayViewHolder holder, int position) {
        CalendarDay item = days.get(position);

        if (item.getDay() == 0) {
            holder.container.setVisibility(View.INVISIBLE);
            return;
        } else {
            holder.container.setVisibility(View.VISIBLE);
        }

        holder.tvDay.setText(String.valueOf(item.getDay()));

        NumberFormat currencyFormat = NumberFormat.getNumberInstance(Locale.KOREA);

        if (item.getIncome() > 0) {
            holder.tvIncomeSum.setText("+" + currencyFormat.format(item.getIncome()));
            holder.tvIncomeSum.setVisibility(View.VISIBLE);
        } else {
            holder.tvIncomeSum.setVisibility(View.GONE);
        }

        if (item.getExpense() > 0) {
            holder.tvExpenseSum.setText("-" + currencyFormat.format(item.getExpense()));
            holder.tvExpenseSum.setVisibility(View.VISIBLE);
        } else {
            holder.tvExpenseSum.setVisibility(View.GONE);
        }

        if (item.getExpense() <= item.getDailyAllowance()) {
            holder.tvStatus.setText("Good");
            holder.tvStatus.setTextColor(Color.GREEN);
        } else {
            holder.tvStatus.setText("Bad");
            holder.tvStatus.setTextColor(Color.RED);
        }

        holder.container.setOnClickListener(v -> onDayClick.onDayClick(item));

        if (item.isToday()) {
            holder.container.setBackgroundColor(Color.parseColor("#E0F7FA"));
        } else {
            holder.container.setBackgroundColor(Color.WHITE);
        }
    }

    @Override
    public int getItemCount() {
        return days.size();
    }

    static class DayViewHolder extends RecyclerView.ViewHolder {
        TextView tvDay;
        TextView tvIncomeSum;
        TextView tvExpenseSum;
        TextView tvStatus;
        View container;

        DayViewHolder(View view) {
            super(view);
            tvDay = view.findViewById(R.id.tv_day);
            tvIncomeSum = view.findViewById(R.id.tv_income_sum);
            tvExpenseSum = view.findViewById(R.id.tv_expense_sum);
            tvStatus = view.findViewById(R.id.tv_status);
            container = view;
        }
    }
}
