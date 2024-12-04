package com.example.mobile_programming_recipe_recommendations;


import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.DatePicker;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.google.android.material.bottomsheet.BottomSheetDialog;

import java.util.ArrayList;

public class AddIngredients extends AppCompatActivity {
    Integer[] imageID = {R.drawable.apple, R.drawable.brown_seaweed, R.drawable.cabbage, R.drawable.mayonnaise,
            R.drawable.milk, R.drawable.oyster_mushroom, R.drawable.pea, R.drawable.spinach,
            R.drawable.sweet_potato};
    ImageButton return_to_my_refrigerator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.add_ingredients_activity);
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return_to_my_refrigerator = (ImageButton)findViewById(R.id.returnMy_refrigerator);
        return_to_my_refrigerator.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });

        ListView add_ingredent_list = findViewById(R.id.listItem);
        AddIngredentList adapter = new AddIngredentList(this, imageID);
        add_ingredent_list.setAdapter(adapter);

        //_________________________________________________________________________________________
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.add_ingredients), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }
}

class AddIngredentList extends BaseAdapter {
    private Context context;
    private Integer[] imageID;
    ArrayList<Integer> add_ingredient_array = new ArrayList<Integer>();
    ArrayList<String> selectedDate_array = new ArrayList<String>();

    public AddIngredentList(Context c, Integer[] id){
        context = c;
        imageID = id;
    }

    @Override
    public int getCount() {
        return imageID.length;
    }

    @Override
    public Object getItem(int i) {
        return null;
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        if(view == null) {
            view = LayoutInflater.from(context).inflate(R.layout.add_ingredients_individual_list, viewGroup, false);
        }

        ImageView imageView = view.findViewById(R.id.imageView);
        imageView.setImageResource(imageID[i]);

        ImageButton addButton = view.findViewById(R.id.add_button);
        addButton.setOnClickListener(view1 -> dialog(imageID[i]));

        return view;
    }

    private void dialog(Integer imageId){
        BottomSheetDialog bottomSheetDialog = new BottomSheetDialog(context);
        bottomSheetDialog.setContentView(R.layout.dialog);

        DatePicker datePicker = bottomSheetDialog.findViewById(R.id.expiration_date_input);
        ImageButton commitButton = bottomSheetDialog.findViewById(R.id.expiration_date_input_commit);
        if (commitButton != null && datePicker != null) {
            commitButton.setOnClickListener(view -> {
                int day = datePicker.getDayOfMonth();
                int month = datePicker.getMonth() + 1; // Month is 0-based
                int year = datePicker.getYear();
                String selectedDate = year + "." + month + "." + day;

                Intent result_my_refrigerator = new Intent();
                add_ingredient_array.add(imageId);
                selectedDate_array.add(selectedDate);

                result_my_refrigerator.putIntegerArrayListExtra("SelectedNumber", add_ingredient_array);
                result_my_refrigerator.putExtra("SelectedDate", selectedDate_array);
                //결과로 던지면 될 듯하다.
                ((AddIngredients) context).setResult(Activity.RESULT_OK, result_my_refrigerator);

                Toast.makeText(context, "지금까지 " + add_ingredient_array.size() + "개가 추가됐어요!", Toast.LENGTH_SHORT).show();
                bottomSheetDialog.dismiss();
            });
        }

        bottomSheetDialog.show();
    }
}