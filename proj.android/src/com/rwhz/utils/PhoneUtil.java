package com.rwhz.utils;

import java.lang.reflect.Method;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;

import com.rwhz.jni.MiscHelper;

public class PhoneUtil {
	
	private static String TAG = "PhoneUtil";
	
	public static int phoneSigValue = 0;

	public static PhoneInfoVo getPhoneInfoVo(Context context) {
		 PhoneInfoVo phoneInfoVo = new PhoneInfoVo();
		 String imsi = "";
		 String imei ="0000000";
		 String phoneModel = "";
		 String line1Number = "";
		 String simSerialNumber = "";
		 boolean simStatusReady = false;
		try { 
			TelephonyManager telMgr = null;
			 telMgr = (TelephonyManager) context.
						getSystemService(Context.TELEPHONY_SERVICE);
			imei = telMgr.getDeviceId();
			line1Number =telMgr.getLine1Number();
			simSerialNumber = telMgr.getSimSerialNumber();
			phoneModel = android.os.Build.MODEL;
			/***
			 * 5=状态良好
			 * 1=您目前没有SIM卡
			 * 0=SIM卡处于未知状态
			 * 2=用户的SIM卡的PIN码解锁
			 * 3=用户的SIM PUK解锁
			 * 4=要求它们网络的PIN码解锁
			 */
			if(telMgr.getSimState() ==TelephonyManager.SIM_STATE_READY){
				simStatusReady = true;
			}
			imsi = telMgr.getSubscriberId();
			//普通方法获取imsi
			if (imsi==null || "".equals(imsi)) imsi = telMgr.getSimOperator();
			
			Class<?>[] resources = new Class<?>[] {int.class};
			Integer resourcesId = Integer.valueOf(1);
			if (null == imsi || "".equals(imsi)) {
				try {   //利用反射获取    MTK手机
					Method addMethod = telMgr.getClass().getDeclaredMethod("getSubscriberIdGemini", resources);
					addMethod.setAccessible(true);
					imsi = (String) addMethod.invoke(telMgr, resourcesId);
				} catch (Exception e) {
					imsi = null;
				}
			}
			if (imsi==null || "".equals(imsi)) {
				try {   //利用反射获取    展讯手机
					Class<?> c = Class.forName("com.android.internal.telephony.PhoneFactory");
					Method m = c.getMethod("getServiceName", String.class, int.class);
					String spreadTmService = (String) m.invoke(c, Context.TELEPHONY_SERVICE, 1);
					TelephonyManager tm1 = (TelephonyManager) context.getSystemService(spreadTmService);
					imsi = tm1.getSubscriberId();
				} catch (Exception e) {
					imsi = null;
				}
			}
			if (imsi==null || "".equals(imsi)) {
				try {   //利用反射获取    高通手机
					Method addMethod2 = telMgr.getClass().getDeclaredMethod("getSimSerialNumber", resources);
					addMethod2.setAccessible(true);
					imsi = (String) addMethod2.invoke(telMgr, resourcesId);
				} catch (Exception e) {
					imsi = null;
				}
			}
			if (imsi==null || "".equals(imsi)) {
				imsi = "000000";
			}
		} catch (Exception e) {
//			MobileUtil.postErrorLog(context, e.getLocalizedMessage(), "getPhoneInfoVo", null, null);
			imsi =  "000000";
		}
		if(null == imei || TextUtils.isEmpty(imei))
			imei = imsi;
		phoneInfoVo.setSimStatusReady(simStatusReady);
		phoneInfoVo.setImsi(imsi);
		phoneInfoVo.setImei(imei);
		phoneInfoVo.setPhoneModel(phoneModel);
		phoneInfoVo.setLine1Number(line1Number);
		phoneInfoVo.setSimSerialNumber(simSerialNumber);
		return phoneInfoVo;
	}
	
	/**
	 * 获取运营商
	 * @return
	 */
	public static String getOperatorName(Context context){
		PhoneInfoVo phoneInfo = PhoneUtil.getPhoneInfoVo(context);
		String imsi = phoneInfo.getImsi();
		if(!TextUtils.isEmpty(imsi)){
			if(imsi.startsWith("46000") || imsi.startsWith("46002"))
			{
			}else if(imsi.startsWith("46001")){
				return "中国联通";
			}else if(imsi.startsWith("46003")){
				return "中国电信";
			}
		}
		return "";
	}
	
	/**
	 * 获取当前网络类型 WIFI 4G等
	 */
	public static String getNetworkType(Context context)
	{
	    String strNetworkType = "";
	    NetworkInfo networkInfo = ((ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE)).getActiveNetworkInfo();
	    if (networkInfo != null && networkInfo.isConnected())
	    {
	        if (networkInfo.getType() == ConnectivityManager.TYPE_WIFI)
	        {
	            strNetworkType = "WIFI";
	        }
	        else if (networkInfo.getType() == ConnectivityManager.TYPE_MOBILE)
	        {
	            String _strSubTypeName = networkInfo.getSubtypeName();
	            
	            Log.e("cocos2d-x", "Network getSubtypeName : " + _strSubTypeName);
	            
	            // TD-SCDMA   networkType is 17
	            int networkType = networkInfo.getSubtype();
	            switch (networkType) {
	                case TelephonyManager.NETWORK_TYPE_GPRS:
	                case TelephonyManager.NETWORK_TYPE_EDGE:
	                case TelephonyManager.NETWORK_TYPE_CDMA:
	                case TelephonyManager.NETWORK_TYPE_1xRTT:
	                case TelephonyManager.NETWORK_TYPE_IDEN: //api<8 : replace by 11
	                    strNetworkType = "2G";
	                    break;
	                case TelephonyManager.NETWORK_TYPE_UMTS:
	                case TelephonyManager.NETWORK_TYPE_EVDO_0:
	                case TelephonyManager.NETWORK_TYPE_EVDO_A:
	                case TelephonyManager.NETWORK_TYPE_HSDPA:
	                case TelephonyManager.NETWORK_TYPE_HSUPA:
	                case TelephonyManager.NETWORK_TYPE_HSPA:
	                case TelephonyManager.NETWORK_TYPE_EVDO_B: //api<9 : replace by 14
	                case TelephonyManager.NETWORK_TYPE_EHRPD:  //api<11 : replace by 12
	                case TelephonyManager.NETWORK_TYPE_HSPAP:  //api<13 : replace by 15
	                    strNetworkType = "3G";
	                    break;
	                case TelephonyManager.NETWORK_TYPE_LTE:    //api<11 : replace by 13
	                    strNetworkType = "4G";
	                    break;
	                default:
	                    // http://baike.baidu.com/item/TD-SCDMA 中国移动 联通 电信 三种3G制式
	                    if (_strSubTypeName.equalsIgnoreCase("TD-SCDMA") || _strSubTypeName.equalsIgnoreCase("WCDMA") || _strSubTypeName.equalsIgnoreCase("CDMA2000")) 
	                    {
	                        strNetworkType = "3G";
	                    }
	                    else
	                    {
	                        strNetworkType = _strSubTypeName;
	                    }
	                    
	                    break;
	             }
	             
	            Log.e("cocos2d-x", "Network getSubtype : " + Integer.valueOf(networkType).toString());
	        }
	    }
	    
	    Log.e("cocos2d-x", "Network Type : " + strNetworkType);
	    
	    return strNetworkType;
	}
	
	//获取wifi信号强度
	public static int getWifiSingLevel(Context context){
        try {  
    		WifiManager wifiMg = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);  
            WifiInfo wifiInfo = wifiMg.getConnectionInfo();  
            int nWSig = WifiManager.calculateSignalLevel(wifiInfo.getRssi(), 100);  
//            Log.e(TAG, "new SSID : " + wifiInfo.getSSID() + "   signal strength : "+wifiInfo.getRssi() + "   强度:" + nWSig);  
            return nWSig;
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
        return 0;
	}
	
	public static void putSingValue(Context mContext){
		 int level = phoneSigValue;
		   //判断当前是4G还是WIFI来区分显示什么信号
	     String netType = PhoneUtil.getNetworkType(mContext);
	     if(netType.equalsIgnoreCase("WIFI")){
	    	 level = PhoneUtil.getWifiSingLevel(mContext);
	    	 Log.e(TAG, "WIFI信号强度:" + level);  
	    	 
	     }else{
	    	 Log.e(TAG, "手机信号强度:" + phoneSigValue);  
	     }
	     
	     JSONObject json = new JSONObject();
	     try {
			json.put("level", level);
			json.put("netType", netType);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    MiscHelper.map.put("phone_state", json.toString());
	}
	
}

