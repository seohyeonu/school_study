package com.example.mp_chapter10_1;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridLayout;
import android.widget.GridView;
import android.widget.ImageView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity {

    public class MyGridAdapter extends BaseAdapter{
        Context context;
        Integer[] posterID = {R.drawable.arctic_fox, R.drawable.camel, R.drawable.fennec_fox,
                R.drawable.meerkat, R.drawable.panda, R.drawable.parrot,
                R.drawable.penguin, R.drawable.polar_bear, R.drawable.seal, R.drawable.tiger,
                R.drawable.arctic_fox, R.drawable.camel, R.drawable.fennec_fox,
                R.drawable.meerkat, R.drawable.panda, R.drawable.parrot,
                R.drawable.penguin, R.drawable.polar_bear, R.drawable.seal, R.drawable.tiger,
                R.drawable.arctic_fox, R.drawable.camel, R.drawable.fennec_fox,
                R.drawable.meerkat, R.drawable.panda, R.drawable.parrot,
                R.drawable.penguin, R.drawable.polar_bear, R.drawable.seal, R.drawable.tiger};

        String[] posterTitle = {"북극 여우", "낙타", "사막여우", "미어캣", "래서판다", "앵무새", "팽귄", "북극곰", "물개", "호랑이",
                "북극 여우", "낙타", "사막여우", "미어캣", "래서판다", "앵무새", "팽귄", "북극곰", "물개", "호랑이",
                "북극 여우", "낙타", "사막여우", "미어캣", "래서판다", "앵무새", "팽귄", "북극곰", "물개", "호랑이"};

        public MyGridAdapter(Context c){
            context = c;
        }
        @Override
        public int getCount() {
            return posterID.length;
        }

        @Override
        public Object getItem(int i) {
            return null;
        }

        @Override
        public long getItemId(int i) {
            return 0;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ImageView imageview = new ImageView(context);
            imageview.setLayoutParams(new GridView.LayoutParams(200, 300));
            imageview.setScaleType(ImageView.ScaleType.FIT_CENTER);
            imageview.setPadding(5, 5, 5, 5);

            imageview.setImageResource(posterID[position]);

            final int pos = position;
            imageview.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    View dialogView = View.inflate(MainActivity.this, R.layout.dialog, null);
                    AlertDialog.Builder dlg = new AlertDialog.Builder(MainActivity.this);
                    ImageView ivPoster = dialogView.findViewById(R.id.ivPoster);

                    ivPoster.setImageResource(posterID[pos]);
                    dlg.setTitle(posterTitle[pos]);
                    dlg.setView(dialogView);
                    dlg.setNegativeButton("닫기", null);
                    dlg.show();

                }
            });

            return imageview;
        }



    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        final GridView gv = (GridView) findViewById(R.id.gridView1);
        MyGridAdapter gAdapter = new MyGridAdapter(this);
        gv.setAdapter(gAdapter);


    }
}