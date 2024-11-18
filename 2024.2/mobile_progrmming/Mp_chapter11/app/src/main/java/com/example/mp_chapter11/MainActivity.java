package com.example.mp_chapter11;

import android.database.Cursor;
import android.database.DatabaseUtils;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import androidx.appcompat.app.AppCompatActivity;


public class MainActivity extends AppCompatActivity {

    private EditText editTextName, editTextId;
    private Button buttonDelete;
    private Button buttonUpdate;
    private ListView listView;
    private DatabaseHelper dbHelper;
    private long selectedId = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button search = findViewById(R.id.search);
        Button buttonRegister = findViewById(R.id.add);
        listView = findViewById(R.id.list_item);

        dbHelper = new DatabaseHelper(this);

        // 등록 버튼 클릭 이벤트
        buttonRegister.setOnClickListener(v -> {
            String name = editTextName.getText().toString();
            String studentId = editTextId.getText().toString();
            if (!name.isEmpty() && !studentId.isEmpty()) {
                dbHelper.insertData(name, studentId);
                loadData();
                clearInputs();
            }
        });

        // 수정 버튼 클릭 이벤트
        buttonUpdate.setOnClickListener(v -> {
            String name = editTextName.getText().toString();
            String studentId = editTextId.getText().toString();
            if (selectedId != -1 && !name.isEmpty() && !studentId.isEmpty()) {
                dbHelper.updateData(selectedId, name, studentId);
                loadData();
                clearInputs();
            }
        });

        // 삭제 버튼 클릭 이벤트
        buttonDelete.setOnClickListener(v -> {
            if (selectedId != -1) {
                dbHelper.deleteData(selectedId);
                loadData();
                clearInputs();
            }
        });

        // 리스트뷰 아이템 클릭 이벤트
        listView.setOnItemClickListener((adapterView, view, position, id) -> {
            Cursor cursor = (Cursor) adapterView.getItemAtPosition(position);
            selectedId = cursor.getLong(cursor.getColumnIndex("_id"));
            editTextName.setText(cursor.getString(cursor.getColumnIndex("name")));
            editTextId.setText(cursor.getString(cursor.getColumnIndex("student_id")));
            buttonDelete.setEnabled(true);
            buttonUpdate.setEnabled(true);
        });

        loadData();
    }

    private void loadData() {
        Cursor cursor = dbHelper.getData();
        String[] from = {"_id", "name", "student_id"};
        int[] to = {R.id.textViewIndex, R.id.textViewName, R.id.textViewId};
        SimpleCursorAdapter adapter = new SimpleCursorAdapter(this, R.layout.list_item, cursor, from, to, 0);
        listView.setAdapter(adapter);
    }

    private void clearInputs() {
        editTextName.setText("");
        editTextId.setText("");
        selectedId = -1;
        buttonDelete.setEnabled(false);
        buttonUpdate.setEnabled(false);
    }
}