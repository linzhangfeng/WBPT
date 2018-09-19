
package com.mogujie.tt.imservice.support.audio;

import android.content.Context;
import android.media.AudioManager;

import java.io.File;

import com.rwhz.api.tt.VoiceManager;

public class AudioPlayerHandler{
    public String currentPlayPath = null;
    private SpeexDecoder speexdec = null;
    private Thread th = null;

    private static AudioPlayerHandler instance = null;

    public static  AudioPlayerHandler getInstance() {
        if (null == instance) {
            synchronized(AudioPlayerHandler.class){
                instance = new AudioPlayerHandler();
            }
        }
        return instance;
    }


    //语音播放的模式
    public  void setAudioMode(int mode,Context ctx) {
//        AudioManager audioManager = (AudioManager) ctx.getSystemService(Context.AUDIO_SERVICE);
//        audioManager.setMode(mode);
    }

    /**messagePop调用*/
    public int getAudioMode(Context ctx) {
        AudioManager audioManager = (AudioManager) ctx.getSystemService(Context.AUDIO_SERVICE);
        return audioManager.getMode();
    }

    public void clear(){
        if (isPlaying()){
            stopPlayer();
        }
        instance = null;
    }


    private AudioPlayerHandler() {
    }

    /**
     * yingmu modify
     * speexdec 由于线程模型
     * */
    public interface AudioListener{
        public void onStop();
    }

    private AudioListener audioListener;

    public void setAudioListener(AudioListener audioListener) {
        this.audioListener = audioListener;
    }

    private void stopAnimation(){
        if(audioListener!=null){
            audioListener.onStop();
        }
    }

    public void onEventMainThread(AudioEvent audioEvent){
        switch (audioEvent){
            case AUDIO_STOP_PLAY:{
                currentPlayPath = null;
                stopPlayer();
            }break;
        }
    }

    public void stopPlayer() {
        try {
            if (null != th) {
                th.interrupt();
                th = null;
                Thread.currentThread().interrupt();
            } else {
            }
        } catch (Exception e) {
        }finally {
            stopAnimation();
        }
    }

    public boolean isPlaying() {
        return null != th;
    }

    public void startPlay(String filePath) {
        this.currentPlayPath = filePath;
        try {
        	File src_file = new File(this.currentPlayPath);
        	if (!src_file.exists())
        	{
        		VoiceManager.getInstance().playNoFile(currentPlayPath);
        		return;
        	}
            speexdec = new SpeexDecoder(filePath);
            RecordPlayThread rpt = new RecordPlayThread();
            if (null == th)
                th = new Thread(rpt);
            th.start();
        } catch (Exception e) {
            stopAnimation();
        }
    }

    class RecordPlayThread extends Thread {
        public void run() {
            try {
                if (null != speexdec)
                    speexdec.decode();
            } catch (Exception e) {
                stopAnimation();
            }
        }
    };

    public String getCurrentPlayPath() {
        return currentPlayPath;
    }
}
