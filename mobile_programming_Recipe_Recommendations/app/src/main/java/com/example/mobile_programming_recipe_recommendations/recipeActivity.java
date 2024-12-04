package com.example.mobile_programming_recipe_recommendations;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.viewpager2.adapter.FragmentStateAdapter;
import androidx.viewpager2.widget.ViewPager2;

public class recipeActivity extends AppCompatActivity  {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_food);
        setTitle("레시피 내용 화면");

        Intent intent = getIntent();
        String foodName = intent.getStringExtra("foodName");
        int foodIdx = intent.getIntExtra("foodIdx", 0);

        Integer foodImgID[] = {R.drawable.noodle, R.drawable.pork, R.drawable.soup, R.drawable.readbean, R.drawable.kimchrice, R.drawable.fishsoup, R.drawable.meatveg, R.drawable.shin};
        Integer foodExID[] = {R.string.noodle_ex, R.string.pork_ex, R.string.beafsoup_ex, R.string.readbean_ex, R.string.kimchr_ex, R.string.fishsoup_ex, R.string.meatveg_ex, R.string.shint_ex};
        Integer foodReID[] = {R.string.noodle_recipe, R.string.pork_recipe, R.string.beafsoup_recipe, R.string.readbean_recipe, R.string.kimchr_recipe, R.string.fishsoup_recipe, R.string.meatveg_recipe, R.string.shint_recipe};

        TextView foodN = (TextView) findViewById(R.id.foodName);
        TextView foodEx = (TextView) findViewById(R.id.explainText);
        TextView foodRe = (TextView) findViewById(R.id.recipeText);
        ImageView foodI = (ImageView) findViewById(R.id.foodImg);

        foodN.setText(foodName);
        foodEx.setText(foodExID[foodIdx]);
        foodRe.setText(foodReID[foodIdx]);
        foodI.setImageResource(foodImgID[foodIdx]);

        ViewPager2 viewPager = findViewById(R.id.reviewBanner);

        Integer[] noodler = {R.drawable.noodle_review, R.drawable.noodle_review2};
        Integer[] porkr = {R.drawable.pork_review1, R.drawable.pork_review2};
        Integer[] soupr = {R.drawable.ricesoup_review1, R.drawable.ricesoup_review2};
        Integer[] readbeanr = {R.drawable.readbean_review1, R.drawable.readbean_review2};
        Integer[] kimchr = {R.drawable.kimchr_review1, R.drawable.kimchr_review2, R.drawable.kimchr_review3};
        Integer[] fishsoup = {R.drawable.fishsoup_review1, R.drawable.fishsoup_review2};
        Integer[] meatveg = {R.drawable.meatveg_review1, R.drawable.meatveg_review2, R.drawable.meatveg_review3};
        Integer[] shint = {R.drawable.shint_review1, R.drawable.shint_review2, R.drawable.shint_review3, R.drawable.shint_review4};

        Integer[][] foodReview = {noodler, porkr, soupr, readbeanr, kimchr, fishsoup, meatveg, shint};

        BannerAdapter bannerAdapter = new BannerAdapter(foodReview[foodIdx]);
        viewPager.setAdapter(bannerAdapter);

    }
}
