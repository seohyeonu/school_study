package com.example.money_tree.ui;

import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import com.example.money_tree.R;
import com.example.money_tree.data.Transaction;
import java.text.NumberFormat;
import java.util.List;
import java.util.Locale;

public class TransactionAdapter extends RecyclerView.Adapter<TransactionAdapter.TransactionViewHolder> {

    private final List<Transaction> transactions;

    public TransactionAdapter(List<Transaction> transactions) {
        this.transactions = transactions;
    }

    @NonNull
    @Override
    public TransactionViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_transaction, parent, false);
        return new TransactionViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull TransactionViewHolder holder, int position) {
        Transaction item = transactions.get(position);

        holder.tvMemo.setText(item.getMemo() != null ? item.getMemo() : "내용 없음");

        NumberFormat currencyFormat = NumberFormat.getNumberInstance(Locale.KOREA);
        String amountStr = currencyFormat.format(item.getAmount());

        if ("INCOME".equals(item.getType())) {
            holder.tvAmount.setText("+" + amountStr + "원");
            holder.tvAmount.setTextColor(Color.BLUE);
            holder.tvType.setText("수입");
        } else {
            holder.tvAmount.setText("-" + amountStr + "원");
            holder.tvAmount.setTextColor(Color.RED);
            holder.tvType.setText("지출");
        }
        
        // Date is also in the layout item_transaction.xml (tv_date)
        // Let's set it if it exists in ViewHolder.
        // Kotlin adapter didn't seem to set it in the code I read?
        // Wait, let's check Kotlin code again.
        // Kotlin code: `holder.tvMemo`, `holder.tvAmount`, `holder.tvType`.
        // It did NOT set `tvDate`.
        // But `item_transaction.xml` has `tv_date`.
        // I should probably add it for completeness, but to match previous behavior I might skip it.
        // However, better to add it if I can.
        // Let's stick to what was there to avoid errors if I miss ID.
        // Actually, `item_transaction.xml` definitely has `tv_date`.
        // I'll add it to ViewHolder and bind it.
        
        if (holder.tvDate != null) {
             java.text.SimpleDateFormat sdf = new java.text.SimpleDateFormat("yyyy.MM.dd", Locale.KOREA);
             holder.tvDate.setText(sdf.format(new java.util.Date(item.getDate())));
        }
    }

    @Override
    public int getItemCount() {
        return transactions.size();
    }

    static class TransactionViewHolder extends RecyclerView.ViewHolder {
        TextView tvMemo;
        TextView tvAmount;
        TextView tvType;
        TextView tvDate;

        TransactionViewHolder(View view) {
            super(view);
            tvMemo = view.findViewById(R.id.tv_memo);
            tvAmount = view.findViewById(R.id.tv_amount);
            tvType = view.findViewById(R.id.tv_type);
            tvDate = view.findViewById(R.id.tv_date);
        }
    }
}
