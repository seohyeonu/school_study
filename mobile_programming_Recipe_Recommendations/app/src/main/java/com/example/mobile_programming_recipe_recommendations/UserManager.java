package com.example.mobile_programming_recipe_recommendations;

import java.util.HashMap;

public class UserManager {
    private static UserManager instance;
    private HashMap<String, User> users; // 사용자 목록
    private User currentUser; // 현재 로그인된 사용자

    private UserManager() {
        users = new HashMap<>();
    }

    public static UserManager getInstance() {
        if (instance == null) {
            instance = new UserManager();
        }
        return instance;
    }

    public void addUser(User user) {
        users.put(user.getUsername(), user);
    }

    public User getUser(String username) {
        return users.get(username);
    }

    public void setCurrentUser(String username) {
        currentUser = users.get(username);
    }

    public User getCurrentUser() {
        return currentUser;
    }
}
