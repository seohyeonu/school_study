package com.example.mobile_programming_recipe_recommendations;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.HashMap;

public class RecommendActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_recommend);
        setTitle("추천된 레시피");

        ListView listView = (ListView) findViewById(R.id.listView);
        HashMap<String, String[]> foodMap = new HashMap<>();

        String foodName[] = {
                "바지락 칼국수", "돼지고기 덮밥", "소고기 떡국", "단팥죽", "김치볶음밥", "어묵국", "소세지야채볶음", "신라면 투움바"
        };

        String[][] ingredients = {
                {"바지락", "칼국수 면", "다시마", "멸치", "대파", "애호박", "양파"},
                {"돼지고기", "양파", "당근", "대파", "간장", "설탕", "고추장", "참기름", "밥", "깨"},
                {"소고기", "떡국떡", "달걀", "대파", "김가루", "국간장", "다진 마늘", "참기름", "소금", "후추"},
                {"팥", "찹쌀", "설탕", "소금", "물", "밤", "대추", "통깨"},
                {"밥", "김치", "햄", "달걀", "식용유", "참기름", "김가루", "대파"},
                {"어묵", "무", "대파", "고춧가루", "다시마", "멸치", "간장", "설탕"},
                {"소세지", "파프리카", "양파", "당근", "마늘", "올리브유", "소금", "후추"},
                {"신라면", "우유", "크림", "치즈", "베이컨", "양파", "마늘", "파슬리"}
        };

        String[] userIngredients = {"신라면", "마늘", "우유", "양파", "밥", "대파", "달걀", "김치", "참기름", "돼지고기", "당근"};

        Integer[] imgID = {
                R.drawable.button_noodle, R.drawable.button_pork, R.drawable.button_beafsoup, R.drawable.button_redbean,
                R.drawable.button_kimchr2, R.drawable.button_fishsoup2, R.drawable.button_meatveg2, R.drawable.button_shin2
        };

        for (int i = 0; i < foodName.length; i++) {
            foodMap.put(foodName[i], ingredients[i]);
        }

        ArrayList<String> matchName = new ArrayList<>();
        ArrayList<Integer> matchIndex = new ArrayList<>();

        for (int i = 0; i < foodName.length; i++) {
            String food = foodName[i];
            String[] foodIngredients = foodMap.get(food);

            int cnt = 0;
            for (String ingredient : foodIngredients) {
                for (String userIngredient : userIngredients) {
                    if (ingredient.equals(userIngredient)) {
                        cnt++;
                        break;
                    }
                }
            }

            if (cnt >= Math.ceil(foodIngredients.length / 2.0)) {
                matchName.add(food);
                matchIndex.add(i);
            }
        }

        Integer[] recommendImgID = new Integer[matchName.size()];

        for (int i = 0; i < matchIndex.size(); i++) {
            recommendImgID[i] = imgID[matchIndex.get(i)];
        }

        RecipeAdapter adapter = new RecipeAdapter(this, recommendImgID);
        listView.setAdapter(adapter);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                String foodN = matchName.get(position);
                int foodIdx = matchIndex.get(position);

                Intent intent = new Intent(RecommendActivity.this, recipeActivity.class);
                intent.putExtra("foodIdx", foodIdx);
                intent.putExtra("foodName", foodN);
                startActivity(intent);
            }
        });
    }
}