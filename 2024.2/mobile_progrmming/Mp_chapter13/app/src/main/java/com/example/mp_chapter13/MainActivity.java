package com.example.mp_chapter13;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    private static  final  String FILE_NAME = "MyText.txt";
    private EditText editText;
    private Button saveButton, newButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        editText = findViewById(R.id.editText);
        saveButton = findViewById(R.id.btnSave);
        newButton = findViewById(R.id.btnNew);

        loadContent();

        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                saveContent();
            }
        });

        newButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                newContent();
            }
        });


    }

    private  void loadContent(){
        try(FileInputStream fis = openFileInput(FILE_NAME);
            InputStreamReader isr = new InputStreamReader(fis);
            BufferedReader br = new BufferedReader(isr)){

            StringBuilder content = new StringBuilder();
            String line;
            while ((line = br.readLine()) != null){
                content.append(line).append("\n");
            }
            editText.setText(content.toString().trim());
            Toast.makeText(this, "작업 중이던 파일을 불러 왔습니다." , Toast.LENGTH_SHORT).show();
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    private void saveContent(){
        String textTosave = editText.getText().toString();
        try(FileOutputStream fos = openFileOutput(FILE_NAME, MODE_PRIVATE)){
            fos.write(textTosave.getBytes());
            Toast.makeText(this, "저장되었습니다.", Toast.LENGTH_SHORT).show();
        } catch (Exception e){
            e.printStackTrace();
            Toast.makeText(this, "저장에 실패 하였습니다.", Toast.LENGTH_SHORT).show();
        }
    }

    private void newContent(){
        String textTosave = "";
        try(FileOutputStream fos = openFileOutput(FILE_NAME, MODE_PRIVATE)){
            fos.write(textTosave.getBytes());
            Toast.makeText(this, "초기화 되었습니다.", Toast.LENGTH_SHORT).show();
        } catch (Exception e){
            e.printStackTrace();
            Toast.makeText(this, "저장에 실패 하였습니다.", Toast.LENGTH_SHORT).show();
        }
    }
}