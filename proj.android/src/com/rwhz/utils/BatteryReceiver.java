package com.rwhz.utils;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.rwhz.jni.MiscHelper;

public class BatteryReceiver extends BroadcastReceiver{


	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		//判断它是否是为电量变化的Broadcast Action
		if(Intent.ACTION_BATTERY_CHANGED.equals(intent.getAction())){
			//获取当前电量
			int level = intent.getIntExtra("level", 0);
			//电量的总刻度
			int scale = intent.getIntExtra("scale", 100);
			//把它转成百分比
			int pl = (level*100)/scale;
			System.out.println("电池电量为"+ pl +"%");
			JSONObject json = new JSONObject();
		     try {
				json.put("level", pl);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			MiscHelper.map.put("power_state", json.toString());
		}
	}

}
