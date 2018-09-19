package com.rwhz.utils;

import android.content.Context;
import android.telephony.PhoneStateListener;
import android.telephony.SignalStrength;

public class PhoneSigLevelReceiver extends PhoneStateListener{
	
	private Context mContext;
	
	public PhoneSigLevelReceiver(Context context){
		this.mContext = context;
	}
	
	private static String TAG = "PhoneSigLevelReceiver";

	public void onSignalStrengthsChanged(SignalStrength signalStrength)
	{
	     int level = signalStrength.getGsmSignalStrength();//获取信号强度。
	     
	     PhoneUtil.phoneSigValue = level;
	}
}