package com.example.mobile_programming_recipe_recommendations;

import android.animation.AnimatorInflater;
import android.animation.AnimatorSet;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class Recommend extends AppCompatActivity {

    public class MyGridAdapter extends BaseAdapter {
        Context context;

        Integer[] frontImages = {
                R.drawable.button_side, R.drawable.button_noodle, R.drawable.button_soup,
                R.drawable.button_snack, R.drawable.button_heart, R.drawable.button_main,
                R.drawable.button_salad, R.drawable.button_night, R.drawable.button_else
        };

        String[] backTexts = {
                "사이드 메뉴", "국수", "스프",
                "간식", "하트", "메인 요리",
                "샐러드", "야식", "기타"
        };

        // 카드 상태 배열: true = 뒷면, false = 앞면
        boolean[] cardStates = new boolean[frontImages.length];

        public MyGridAdapter(Context c) {
            context = c;
        }

        public void setBackTexts(String[] backTexts) {
            this.backTexts = backTexts;
        }

        @Override
        public int getCount() {
            return frontImages.length;
        }

        @Override
        public Object getItem(int position) {
            return null;
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            View gridItemView = View.inflate(context, R.layout.grid_item, null);

            // 앞면과 뒷면 초기화
            ImageView frontImage = gridItemView.findViewById(R.id.front_image);
            TextView backText = gridItemView.findViewById(R.id.back_text);

            // 앞면 이미지와 뒷면 텍스트 설정
            frontImage.setImageResource(frontImages[position]);
            backText.setText(backTexts[position]);

            // GridView의 각 아이템 크기 설정
            gridItemView.setLayoutParams(new GridView.LayoutParams(200, 200)); // Image 크기와 동일

            // 상태에 따라 초기화
            if (cardStates[position]) {
                frontImage.setVisibility(View.INVISIBLE);
                backText.setVisibility(View.VISIBLE);
            } else {
                frontImage.setVisibility(View.VISIBLE);
                backText.setVisibility(View.INVISIBLE);
            }

            return gridItemView;
        }

        // 모든 카드를 뒤집는 메서드
        public void flipAllCards(GridView gridView) {
            for (int i = 0; i < gridView.getChildCount(); i++) {
                View itemView = gridView.getChildAt(i);

                if (itemView != null) {
                    ImageView frontImage = itemView.findViewById(R.id.front_image);
                    TextView backText = itemView.findViewById(R.id.back_text);

                    // 각 카드에 대한 뒤집기 애니메이션 실행
                    flipCard(frontImage, backText, i);
                }
            }
        }

        // 카드 뒤집기 애니메이션
        private void flipCard(ImageView frontImage, TextView backText, int position) {
            AnimatorSet frontAnim = (AnimatorSet) AnimatorInflater.loadAnimator(context, R.animator.flip_front);
            AnimatorSet backAnim = (AnimatorSet) AnimatorInflater.loadAnimator(context, R.animator.flip_back);

            if (frontImage.getVisibility() == View.VISIBLE) {
                frontAnim.setTarget(frontImage);
                backAnim.setTarget(backText);

                frontAnim.start();
                backAnim.start();

                frontImage.setVisibility(View.INVISIBLE);
                backText.setVisibility(View.VISIBLE);

                // 카드 상태 업데이트
                cardStates[position] = true;
            } else {
                backAnim.setTarget(frontImage);
                frontAnim.setTarget(backText);

                backAnim.start();
                frontAnim.start();

                backText.setVisibility(View.INVISIBLE);
                frontImage.setVisibility(View.VISIBLE);

                // 카드 상태 업데이트
                cardStates[position] = false;
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        GridView gv = findViewById(R.id.grid);
        MyGridAdapter gAdapter = new MyGridAdapter(this);
        gv.setAdapter(gAdapter);

        gv.setOnItemClickListener((parent, view, position, id) -> {
            if (position == 0) {
                String[] sideMenu = {
                        "김치전", "감자전", "잡채",
                        "떡볶이", "계란찜", "무침",
                        "나물", "콩나물무침", "두부조림"
                };
                gAdapter.setBackTexts(sideMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            } else if(position == 1){
                String[] noodleMenu = {
                        "비빔국수", "잔치국수", "라면",
                        "칼국수", "우동", "스파게티",
                        "쫄면", "짜장면", "짬뽕"
                };
                gAdapter.setBackTexts(noodleMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
            else if(position == 2){
                String[] soupMenu = {
                        "된장찌개", "김치찌개", "순두부찌개",
                        "부대찌개", "미역국", "갈비탕",
                        "삼계탕", "떡국", "육개장"
                };
                gAdapter.setBackTexts(soupMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
            else if(position == 3){
                String[] snackMenu = {
                        "핫도그", "떡볶이", "튀김",
                        "만두", "어묵", "샌드위치",
                        "쿠키", "초코파이", "감자튀김"
                };
                gAdapter.setBackTexts(snackMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
            else if(position == 4){
                String[] sideMenu = {
                        "김치전", "감자전", "잡채",
                        "떡볶이", "계란찜", "무침",
                        "나물", "콩나물무침", "두부조림"
                };
                gAdapter.setBackTexts(sideMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
            else if(position == 5){
                String[] mainMenu = {
                        "불고기", "갈비찜", "닭볶음탕",
                        "수육", "치킨", "삼겹살",
                        "제육볶음", "오리구이", "탕수육"
                };

                gAdapter.setBackTexts(mainMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
            else if(position == 6){
                String[] saladMenu = {
                        "그린샐러드", "과일샐러드", "감자샐러드",
                        "닭가슴살샐러드", "파스타샐러드", "콥샐러드",
                        "단호박샐러드", "고구마샐러드", "참치샐러드"
                };
                gAdapter.setBackTexts(saladMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
            else if(position == 7){
                String[] nightMenu = {
                        "치킨", "피자", "라면",
                        "떡볶이", "족발", "곱창",
                        "소고기구이", "삼겹살", "야채볶음밥"
                };
                gAdapter.setBackTexts(nightMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 0); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
            else if(position == 8){
                String[] otherMenu = {
                        "카레", "리조또", "햄버거",
                        "오므라이스", "타코", "스테이크",
                        "크림파스타", "볶음밥", "샤브샤브"
                };
                gAdapter.setBackTexts(otherMenu);
                gAdapter.flipAllCards(gv);

                // 카드 상태를 갱신한 뒤 데이터 변경
                gv.postDelayed(() -> gAdapter.notifyDataSetChanged(), 150); // 애니메이션 후 데이터 갱신

                Toast.makeText(Recommend.this, "사이드 메뉴를 선택했습니다!", Toast.LENGTH_SHORT).show();
            }
        });
    }
}