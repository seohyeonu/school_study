package com.example.mobile_programming_recipe_recommendations;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ListAdapter;
import android.widget.ListView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class RecipeView extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        setTitle("레시피 보기");

        ListView listView = (ListView) findViewById(R.id.listView);

        String foodName[] = {
                "바지락 칼국수", "돼지고기 덮밥", "소고기 떡국", "단팥죽", "김치볶음밥", "어묵국", "소세지야채볶음", "신라면 투움바"
        };

        Integer[] imgID = {
                R.drawable.button_noodle, R.drawable.button_pork, R.drawable.button_beafsoup, R.drawable.button_redbean,
                R.drawable.button_kimchr2, R.drawable.button_fishsoup2, R.drawable.button_meatveg2, R.drawable.button_shin2
        };

        RecipeAdapter adapter = new RecipeAdapter(this, imgID);
        listView.setAdapter(adapter);

        Button btn = (Button) findViewById(R.id.btn);

        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent1 = new Intent(RecipeView.this, RecommendActivity.class);
                startActivity(intent1);
            }
        });

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                String foodN = foodName[position];
                int foodIdx = position;

                Intent intent = new Intent(RecipeView.this, recipeActivity.class);
                intent.putExtra("foodIdx", foodIdx);
                intent.putExtra("foodName", foodN);
                startActivity(intent);
            }
        });
    }


}
