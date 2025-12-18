package com.example.money_tree.data;

import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity(tableName = "transactions")
public class Transaction {
    @PrimaryKey(autoGenerate = true)
    private long id;
    private long date; // Timestamp
    private String type; // "INCOME" or "EXPENSE"
    private long amount;
    private String memo;
    private long createdAt;

    public Transaction(long date, String type, long amount, String memo, long createdAt) {
        this.date = date;
        this.type = type;
        this.amount = amount;
        this.memo = memo;
        this.createdAt = createdAt;
    }

    // Getters and Setters
    public long getId() { return id; }
    public void setId(long id) { this.id = id; }

    public long getDate() { return date; }
    public void setDate(long date) { this.date = date; }

    public String getType() { return type; }
    public void setType(String type) { this.type = type; }

    public long getAmount() { return amount; }
    public void setAmount(long amount) { this.amount = amount; }

    public String getMemo() { return memo; }
    public void setMemo(String memo) { this.memo = memo; }

    public long getCreatedAt() { return createdAt; }
    public void setCreatedAt(long createdAt) { this.createdAt = createdAt; }
}
