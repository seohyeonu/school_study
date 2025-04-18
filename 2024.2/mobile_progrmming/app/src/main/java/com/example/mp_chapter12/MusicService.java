package com.example.mp_chapter12;

import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import java.security.Provider;
import java.util.List;
import java.util.Map;

public class MusicService extends Service {
        private static final String TAG = "MusicService";
        MediaPlayer player;

        @Override
        public IBinder onBind(Intent intent){
            return null;
    }

    @Override
    public void onCreate(){
            Log.d(TAG, "onCreate()");

            player = MediaPlayer.create(this, R.raw.old_pop);
            player.setLooping(false);
    }

    @Override
    public void onDestroy(){
        Toast.makeText(this, "Music Service가 중지되었습니다.", Toast.LENGTH_LONG).show();
        Log.d(TAG, "onDestory()");
        player.stop();
    }
    @Override
    public  int onStartCommand(Intent intent, int flags, int startId){
            Toast.makeText(this, "Music service가 시작되었습니다.", Toast.LENGTH_LONG).show();
            Log.d(TAG, "onStart()");
            player.start();
            return super.onStartCommand(intent, flags, startId);
    }
}
