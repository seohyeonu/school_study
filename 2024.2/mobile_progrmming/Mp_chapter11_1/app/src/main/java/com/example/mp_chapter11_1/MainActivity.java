package com.example.mp_chapter11_1;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.database.Cursor;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private ListView listView;
    private DatabaseHelper dbHelper;
    private long selectedId = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button buttonAdd = findViewById(R.id.buttonAdd);
        Button buttonSearch = findViewById(R.id.buttonSearch);
        listView = findViewById(R.id.listView);

        dbHelper = new DatabaseHelper(this);

        // "추가하기" 버튼 클릭 이벤트
        buttonAdd.setOnClickListener(v -> showAddDialog());

        // "검색하기" 버튼 클릭 이벤트
        buttonSearch.setOnClickListener(v -> showSearchDialog());

        // 리스트뷰 아이템 길게 누르기 이벤트
        listView.setOnItemLongClickListener((adapterView, view, position, id) -> {
            Cursor cursor = (Cursor) adapterView.getItemAtPosition(position);
            selectedId = cursor.getLong(cursor.getColumnIndex("_id"));
            String name = cursor.getString(cursor.getColumnIndex("name"));
            String studentId = cursor.getString(cursor.getColumnIndex("student_id"));
            showEditDeleteDialog(name, studentId);
            return true;
        });

        loadData();
    }

    // 데이터를 로드하여 ListView에 표시
    private void loadData() {
        Cursor cursor = dbHelper.getData();
        String[] from = {"_id", "name", "student_id"};
        int[] to = {R.id.textViewIndex, R.id.textViewName, R.id.textViewId};
        SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.list_item, cursor, from, to, 0);
        listView.setAdapter(adapter);
    }

    // "추가하기" 다이얼로그
    private void showAddDialog() {
        LayoutInflater inflater = LayoutInflater.from(this);
        View dialogView = inflater.inflate(R.layout.dialog_add, null);
        EditText editTextName = dialogView.findViewById(R.id.editTextName);
        EditText editTextId = dialogView.findViewById(R.id.editTextId);

        new AlertDialog.Builder(this)
                .setTitle("추가하기")
                .setView(dialogView)
                .setPositiveButton("저장", (dialog, which) -> {
                    String name = editTextName.getText().toString();
                    String studentId = editTextId.getText().toString();
                    if (!TextUtils.isEmpty(name) && !TextUtils.isEmpty(studentId)) {
                        dbHelper.insertData(name, studentId);
                        loadData();
                    }
                })
                .setNegativeButton("취소", null)
                .show();
    }

    // "검색하기" 다이얼로그
    private void showSearchDialog() {
        LayoutInflater inflater = LayoutInflater.from(this);
        View dialogView = inflater.inflate(R.layout.dialog_search, null);
        EditText editTextSearch = dialogView.findViewById(R.id.editTextSearch);

        new AlertDialog.Builder(this)
                .setTitle("검색하기")
                .setView(dialogView)
                .setPositiveButton("검색", (dialog, which) -> {
                    String query = editTextSearch.getText().toString();
                    if (!TextUtils.isEmpty(query)) {
                        Cursor cursor = dbHelper.searchData(query);
                        String[] from = {"_id", "name", "student_id"};
                        int[] to = {R.id.textViewIndex, R.id.textViewName, R.id.textViewId};
                        SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.list_item, cursor, from, to, 0);
                        listView.setAdapter(adapter);
                    }
                })
                .setNegativeButton("취소", null)
                .show();
    }

    // "수정/삭제" 다이얼로그
    private void showEditDeleteDialog(String name, String studentId) {
        new AlertDialog.Builder(this)
                .setTitle("옵션 선택")
                .setItems(new String[]{"수정하기", "삭제하기"}, (dialog, which) -> {
                    if (which == 0) {
                        showEditDialog(name, studentId);
                    } else if (which == 1) {
                        dbHelper.deleteData(selectedId);
                        loadData();
                    }
                })
                .show();
    }

    // "수정하기" 다이얼로그
    private void showEditDialog(String oldName, String oldId) {
        LayoutInflater inflater = LayoutInflater.from(this);
        View dialogView = inflater.inflate(R.layout.dialog_add, null);
        EditText editTextName = dialogView.findViewById(R.id.editTextName);
        EditText editTextId = dialogView.findViewById(R.id.editTextId);

        editTextName.setText(oldName);
        editTextId.setText(oldId);

        new AlertDialog.Builder(this)
                .setTitle("수정하기")
                .setView(dialogView)
                .setPositiveButton("저장", (dialog, which) -> {
                    String newName = editTextName.getText().toString();
                    String newId = editTextId.getText().toString();
                    if (!TextUtils.isEmpty(newName) && !TextUtils.isEmpty(newId)) {
                        dbHelper.updateData(selectedId, newName, newId);
                        loadData();
                    }
                })
                .setNegativeButton("취소", null)
                .show();
    }
}
