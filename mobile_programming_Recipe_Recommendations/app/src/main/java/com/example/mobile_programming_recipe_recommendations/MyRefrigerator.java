package com.example.mobile_programming_recipe_recommendations;


import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.DatePicker;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.util.ArrayList;
import java.util.List;

public class MyRefrigerator extends AppCompatActivity {
    ImageButton button_goto_addLayout;
    ArrayList<String> indivualList;
    ListView listView;
    MyRefrigeratorAdapter listAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.my_refrigerator);
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        button_goto_addLayout = (ImageButton)findViewById(R.id.goto_addLayout);
        button_goto_addLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent goto_addLayout = new Intent(MyRefrigerator.this, AddIngredients.class);
                startActivityForResult(goto_addLayout, 1);
            }
        });

        indivualList = new ArrayList<String>();
        listView = (ListView)findViewById(R.id.listItem);

        listAdapter = new MyRefrigeratorAdapter(this, indivualList);
        listView.setAdapter(listAdapter);

        // 작동이 잘 이뤄지기도 하고, 회의 중 유통기한 텍스트를 누르면 되게끔 유도해달라는 요청에 변경하게 됨.
//        //해당 리스트를 길게 누르면 유통기한이 변경되게 하자.
//        listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
//            @Override
//            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int i, long l) {
//                String[] strings = indivualList.get(i).split("\\|");
//
//                // 전달 받은게 정확히 나눠졌는지 확인하자.
//                if (strings.length == 2) {
//                    showDialog(i, strings[1]);
//                }
//
//                return true;
//            }
//        });
        //__________________________________________________________________________________
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.my_refrigerator), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == RESULT_OK && data != null) {
            ArrayList<Integer> selectedNumber = data.getIntegerArrayListExtra("SelectedNumber");
            ArrayList<String> selectedDate = data.getStringArrayListExtra("SelectedDate");
            for(int index = 0 ; index < selectedNumber.size() ; index++) {
                indivualList.add(selectedNumber.get(index) + "|" + selectedDate.get(index));
                listAdapter.notifyDataSetChanged();
            }

        }
    }

    void showDialog(int targetListIndex, String targetListExpriation){
        View dialogView = getLayoutInflater().inflate(R.layout.dialog, null);

        AlertDialog.Builder builder =  new AlertDialog.Builder(this);
        builder.setView(dialogView);
        AlertDialog dialog = builder.create();

        DatePicker datePicker = dialogView.findViewById(R.id.expiration_date_input);
        ImageButton commitButton = dialogView.findViewById(R.id.expiration_date_input_commit);

        String[] dateStrings = targetListExpriation.split("\\.");
        if(dateStrings.length == 3){
            int day = Integer.parseInt(dateStrings[2]);
            int month = Integer.parseInt(dateStrings[1]) - 1;
            int year = Integer.parseInt(dateStrings[0]);
            datePicker.updateDate(year, month, day);
        }

        commitButton.setOnClickListener(view -> {
            int day = datePicker.getDayOfMonth();
            int month = datePicker.getMonth() + 1; // Month is 0-based
            int year = datePicker.getYear();
            String dateModified = year + "." + month + "." + day;

            String[] setStrings = indivualList.get(targetListIndex).split("\\|");
            if(setStrings.length == 2){
                indivualList.set(targetListIndex, setStrings[0] + "|" + dateModified);
                listAdapter.notifyDataSetChanged();
            }

            dialog.dismiss();
        });

        dialog.show();
    }
}

class MyRefrigeratorAdapter extends BaseAdapter{
    private Context context;
    private List<String> item;

    public MyRefrigeratorAdapter(Context context, List<String> item){
        this.context = context;
        this.item = item;
    }


    @Override
    public int getCount() {
        return item.size();
    }

    @Override
    public Object getItem(int i) {
        return item.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        if (view == null) {
            view = LayoutInflater.from(context).inflate(R.layout.my_refrigerator_individual_list, viewGroup, false);
        }

        ImageView imageView = view.findViewById(R.id.imageView);
        ImageButton removeButton = view.findViewById(R.id.remove_button);
        TextView expirationDate = view.findViewById(R.id.expiration_date);

        /*
        위에서 indivualList.add(selectedNumber.get(index) + "|" + selectedDate.get(index)); 로 만들어진걸 바탕으로
        "R.drawable.imageID|2024.11.20" 문자열을 "|" 기준으로 나누어서 아래와 같이 xml에 이미지와 텍스트를 수정해준다.
"         */
        String[] strings = item.get(i).split("\\|");

        // 전달 받은게 정확히 나눠졌는지 확인하자.
        if (strings.length == 2) {
            // 전달 받은 이미지 소스 명으로 이미지 뷰 설정
            imageView.setImageResource(Integer.parseInt(strings[0]));

            // 전달 받은 유통기한으로 텍스트 설정
            expirationDate.setText(strings[1]);


            expirationDate.setOnClickListener(v -> {
                ((MyRefrigerator) context).showDialog(i, strings[1]);
            });
        }
        else { // 원치 않은 데이터를 입력 받으면 리스트 업 하지 말고 날리자.
            item.remove(i);
            notifyDataSetChanged();
        }

        //개별 리스트 안에 빨간 버튼 누르면 그 항목을 제거 됩니다.
        removeButton.setOnClickListener(v -> {
            item.remove(i);
            notifyDataSetChanged();
        });

        return view;
    }
}
