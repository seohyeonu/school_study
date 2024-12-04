package com.example.mobile_programming_recipe_recommendations;

import java.util.ArrayList;

public class User {
    private String username;
    private ArrayList<String> ingredients; // "재료 이름 | 유통기한" 형태의 문자열로 저장

    public User(String username) {
        this.username = username;
        this.ingredients = new ArrayList<>();
    }

    public String getUsername() {
        return username;
    }

    public ArrayList<String> getIngredients() {
        return ingredients;
    }

    public void addIngredient(String ingredient) {
        ingredients.add(ingredient);
    }

    public void removeIngredient(String ingredient) {
        ingredients.remove(ingredient);
    }
}
