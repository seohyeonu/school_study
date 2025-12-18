package com.example.money_tree.data;

import androidx.room.Dao;
import androidx.room.Insert;
import androidx.room.Query;
import java.util.List;

@Dao
public interface TransactionDao {
    @Insert
    void insert(Transaction transaction);

    @Query("SELECT * FROM transactions ORDER BY date DESC, id DESC")
    List<Transaction> getAll();

    @Query("SELECT * FROM transactions WHERE date = :date ORDER BY id DESC")
    List<Transaction> getByDate(long date);

    @Query("SELECT * FROM transactions WHERE date >= :startDate AND date <= :endDate ORDER BY date ASC")
    List<Transaction> getByDateRange(long startDate, long endDate);

    @Query("DELETE FROM transactions")
    void deleteAll();
}
