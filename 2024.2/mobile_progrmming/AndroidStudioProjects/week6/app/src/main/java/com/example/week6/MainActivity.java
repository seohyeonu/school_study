package com.example.week6;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {
    Button add;
    Button miu;
    Button mul;
    Button div;
    Button divResult;
    Button kill;
    EditText first;
    EditText send;
    TextView cal;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        add =(Button) findViewById(R.id.add);
        miu =(Button) findViewById(R.id.miu);
        mul =(Button) findViewById(R.id.mul);
        div =(Button) findViewById(R.id.div);
        divResult =(Button) findViewById(R.id.divResult);
        kill =(Button) findViewById(R.id.kill);
        first = (EditText) findViewById(R.id.first);
        send = (EditText) findViewById(R.id.send);
        cal = (TextView) findViewById(R.id.cal);


        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        add.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
//                String getFrist=first.getText().toString();
//                String getSend=send.getText().toString();
//                int result = Integer.valueOf(getFrist) + Integer.valueOf(getSend);
//                cal.setText("계산 결과 : " + result);
            }
        });
    }
}