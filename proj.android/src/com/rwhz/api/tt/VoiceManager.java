package com.rwhz.api.tt;

import org.json.JSONException;
import org.json.JSONObject;

import android.R.bool;
import android.media.AudioManager;

import com.mogujie.tt.imservice.support.audio.AudioPlayerHandler;
import com.mogujie.tt.imservice.support.audio.AudioRecordHandler;
import com.mogujie.tt.imservice.support.audio.SysConstant;
import com.rwhz.app.MainActivity;
import com.rwhz.jni.MiscHelper;

public class VoiceManager {
	private AudioRecordHandler audioRecorderInstance = null;
	private Thread audioRecorderThread = null;
	private static String audioSavePath;
	private static String curPlayAudio;
	
	private Boolean isPlaying = false;
	private static VoiceManager instance = null;

	public static VoiceManager getInstance() {
		if (null == instance) {
			synchronized (VoiceManager.class) {
				instance = new VoiceManager();
			}
		}
		return instance;
	}
	
	public void init()
	{
		
	}

	public void startRecord(String path) {
		audioSavePath = path;
		if (AudioPlayerHandler.getInstance().isPlaying()){
			AudioPlayerHandler.getInstance().stopPlayer();
		}
		audioRecorderInstance = new AudioRecordHandler(audioSavePath);
		audioRecorderThread = new Thread(audioRecorderInstance);
		audioRecorderInstance.setRecording(true);
		audioRecorderThread.start();
	}

	public void recordStart() {
		MiscHelper.nativeJava2C("chat_sound_record_start_notify", "");
	}

	public void recordOver() {
		int time = (int) audioRecorderInstance.getRecordTime();
		if (time >= SysConstant.MAX_SOUND_RECORD_TIME) {
			// time out
			MiscHelper.nativeJava2C("chat_sound_record_timeout_notify", "");
		} else {

			try {
				JSONObject json = new JSONObject();
				json.put("filename", audioRecorderInstance.fileName);
				json.put("time", time);
				MiscHelper.nativeJava2C("chat_sound_record_over_notify",
						json.toString());
			} catch (JSONException e) {
				e.printStackTrace();
			}

		}
	}

	public void stopRecord() {
		if (!audioRecorderInstance.isRecording()) {
			return;
		}

		if (audioRecorderInstance.isRecording()) {
			audioRecorderInstance.setRecording(false);
			audioRecorderInstance.setForceKill();
		}
	}

	public void playRecord(final String audioPath) {
		// 获取播放路径，播放语音
		if (AudioPlayerHandler.getInstance().isPlaying()) {
			AudioPlayerHandler.getInstance().stopPlayer();
			if (curPlayAudio != null && curPlayAudio.equals(audioPath)) {
				
				curPlayAudio = null;
				return;
			}
		}
		curPlayAudio = audioPath;
		isPlaying = true;
		// 延迟播放
		Thread myThread = new Thread() {
			public void run() {
				try {
					Thread.sleep(200);
					VoiceManager.getInstance().playStart(curPlayAudio);
					AudioPlayerHandler.getInstance().startPlay(audioPath);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		myThread.start();
	}

	public void playOver(String path) {
		if (path == null) path = "";
		if (curPlayAudio != null)
		{
			path = curPlayAudio;
		}
		MiscHelper.nativeJava2C("chat_sound_play_over_notify", path);
		curPlayAudio = null;
		isPlaying = false;
	}

	public void playStart(String path) {
		if (path == null) path = "";
		if (curPlayAudio != null)
		{
			path = curPlayAudio;
		}
		MiscHelper.nativeJava2C("chat_sound_play_start_notify", path);
		isPlaying = true;
	}

	public void playNoFile(String path) {
		if (path == null) path = "";
		if (curPlayAudio != null)
		{
			path = curPlayAudio;
		}
		MiscHelper.nativeJava2C("chat_sound_play_nofile_notify", path);
		curPlayAudio = null;
		AudioPlayerHandler.getInstance().setAudioMode(AudioManager.MODE_NORMAL,MainActivity.app);
	}

	public Boolean isPlaying() {
		return isPlaying;
	}
}
