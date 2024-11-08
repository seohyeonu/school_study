package com.example.mp_chapter9;

import android.content.Context;
import android.media.Image;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.lang.reflect.Array;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    public class MyGridAdapter extends BaseAdapter{
        Context context;
        Integer[] posterID = {
                R.drawable.bear, R.drawable.bird,R.drawable.miacat,
                R.drawable.nacta, R.drawable.northfox, R.drawable.nothbir,
                R.drawable.nothbir, R.drawable.panda, R.drawable.samagfox, R.drawable.tiger
        };
        public  MyGridAdapter(Context c){
            context = c;
        }
        public int getCount(){
            return posterID.length;
        }
        public Object getItem(int arg0){
            return null;
        }
        public  long getItemId(int arg0){
            return 0;
        }

        @Override
        public View getView(int position, View view, ViewGroup viewGroup) {
            ImageView imageview = new ImageView(context);
            imageview.setLayoutParams(new ViewGroup.LayoutParams(200,300));
            imageview.setScaleType(ImageView.ScaleType.FIT_CENTER);
            imageview.setPadding(5,5,5,5);

            imageview.setImageResource(posterID[position]);

            return imageview;
        }
    }
    final  int pos

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
////        String[] strArray = {"리스트 1", "리스트 2", "리스트3", "리스트4", "리시트5", "리스트6", "리스트7", "리스트8", "리스트9", "리스트10"};
//        final ArrayList<String> midList = new ArrayList<String>();
//        ListView list = (ListView) findViewById(R.id.listView);
//
//        final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, midList);
//        list.setAdapter(adapter);
//
////        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, strArray);
//
//        final EditText edtItem = (EditText) findViewById(R.id.edtItem);
//        Button btnAdd = (Button) findViewById(R.id.btu);
//
//        btnAdd.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//
//            }
//        });
//        ltv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
//            @Override
//            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
//                Toast.makeText(getApplicationContext(), strArray[position], Toast.LENGTH_SHORT).show();
//            }
//        });
        final GridView gv = (GridView) findViewById(R.id.gridView1);
        MyGridAdapter gAdapter = new MyGridAdapter(this);
        gv.setAdapter(gAdapter);

        final int pos = postion;
    }


}