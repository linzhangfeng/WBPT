package com.rwhz.jni;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.HashMap;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;

import com.rwhz.api.tt.VoiceManager;
import com.rwhz.app.MainActivity;
import com.rwhz.app.MainListener;
import com.rwhz.baidu.BaiduLocation;
import com.rwhz.utils.Base64Utils;
import com.rwhz.utils.PhoneUtil;
import com.rwhz.utils.RSAUtils;
import com.rwhz.utils.Utils;
import com.rwhz.wx.WXHelper;
import com.tendcloud.tenddata.TCAgent;

public class MiscHelper {

	public static final int VIBRATE_REQUEST = 0x5001;
	public static final int TOAST_LONG_REQUEST = 0x5002;
	public static final int TOAST_SHORT_REQUEST = 0x5003;
	public static final int SHOW_AVATAR_REQUEST = 0x5004;
	public static final int ALERTDIALOG_CZ_REQUEST = 0x5005;
	public static final int Check_NetWork = 0x5006;
	public static final int ASK_EXITGAME = 0x5007;
	public static final int REQUEST_PAY = 0x5008;
	public static final int SHOW_EXIT = 0x5009;
	
	//移动MM SDK 对接
	public static final int MM_ORDER=0x5010;
	
	//微信 sdk 对接
	public static final int WX_SHARE=0x5011;
	
	public static final int CZ=0x5012;
	public static final int InitSDK=0x5013;
	
	public static final int OPEN_Photo = 0x5014;
	public static final int OPEN_CAMERA = 0x5015;
	public static final int COPY_Board = 0x5016;
	
	public static final int ASK_UPDATE = 0x5017;
	
	public static final int WX_SHARE_URL = 0x5018;
	public static final int WX_SHARE_IMAGE = 0x5019;
	
	public static final int EXIT_WHTHOUT_PARAM = 0x5020;
	public static final int WX_SHARE_FRIENDS_CIRCLE_IMAGE = 0x5021;
	public static final int WX_SHARE_FRIENDS_CIRCLE_NORMAL = 0x5022;
	
	
	private static Handler mHandler;
	private static Context mContext;

	public static final int SP_NULL = 0;
	public static final int SP_YiDong = 1;
	public static final int SP_LianTong = 2;
	public static final int SP_DianXin = 3;
	
	public  static boolean IsInitSDK = false;
	
	public static MainListener listener = null;
	
	public static String start_code = "";
	
	public static HashMap<String,String> map = new HashMap();
	
	public static void init(Handler handler, Context context) {
		MiscHelper.mHandler = handler;
		MiscHelper.mContext = context;
	}

	public static void Vibrate(final Activity activity, long milliseconds) {
		Vibrator vib = (Vibrator) activity
				.getSystemService(Service.VIBRATOR_SERVICE);
		vib.vibrate(milliseconds);
	}

	public static void Vibrate(final Activity activity, long[] pattern,
			boolean isRepeat) {
		Vibrator vib = (Vibrator) activity
				.getSystemService(Service.VIBRATOR_SERVICE);
		vib.vibrate(pattern, isRepeat ? 1 : -1);
	}

	public static void setVibrate() {
		Message msg = mHandler.obtainMessage();
		msg.what = VIBRATE_REQUEST;
		msg.sendToTarget();
	}

	public static void showToast(String message, int type) {
		Message msg = mHandler.obtainMessage();
		if (type == 0) {
			msg.what = TOAST_LONG_REQUEST;
		} else {
			msg.what = TOAST_SHORT_REQUEST;
		}
		msg.obj = message;
		msg.sendToTarget();
	}

	public static void alertDialog(String message, int type) {
		Message msg = mHandler.obtainMessage();
		if (type == 0) {
			msg.what = ALERTDIALOG_CZ_REQUEST;
		} else if (type == 100) {
			msg.what = ASK_EXITGAME;
		}
		else if (type == 200) {
			msg.what = SHOW_EXIT;
		}
		msg.obj = message;
		msg.sendToTarget();
	}

	public static void checkNetWork() {
		Message msg = mHandler.obtainMessage();
		msg.what = Check_NetWork;
		msg.sendToTarget();
	}

	public static boolean isOpenNetwork() {
		ConnectivityManager connManager = (ConnectivityManager) MiscHelper.mContext
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connManager.getActiveNetworkInfo() != null) {
			return connManager.getActiveNetworkInfo().isAvailable();
		}

		return false;
	}

	public static void saveFileToSDCard(String fileName,String content)
	{
		if(!MainActivity.hasSdcard())
		{
			return;
		}
		FileOutputStream fos = null;
		try{
			//Environment.getExternalStorageDirectory()。获取sd卡的路径
			File file = new File(Environment.getExternalStorageDirectory(),fileName);
			if(!file.exists())file.createNewFile();
			fos = new FileOutputStream(file,false);
			fos.write(content.getBytes());
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		finally
		{
			try 
			{
				fos.close();
			} 
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}
	}
	
	public static String getContentFromSDCard(String fileName)
	{
		if(!MainActivity.hasSdcard())
		{
			return "";
		}
		String result = "";
		FileInputStream fos = null;
		try{
			//Environment.getExternalStorageDirectory()。获取sd卡的路径
			File file = new File(Environment.getExternalStorageDirectory(),fileName);
			if(file.exists())
			{
				fos = new FileInputStream(file);
				byte[] buffer = new byte[(int)file.length()];
				fos.read(buffer);
				result = new String(buffer);
			}
		}
		catch(Exception e)
		{
//			e.printStackTrace();
		}
		finally
		{
			try 
			{
				if(fos!=null)fos.close();
			} 
			catch (Exception e) 
			{
//				e.printStackTrace();
			}
		}
//		Log.d("content", "---------" + result);
		return result;
	}
	
	public static int isWiFiActive() {      
        ConnectivityManager connectivity = (ConnectivityManager)MiscHelper.mContext.getSystemService(Context.CONNECTIVITY_SERVICE);      
        if (connectivity != null) {      
            NetworkInfo[] infos = connectivity.getAllNetworkInfo();      
            if (infos != null) {      
                for(NetworkInfo ni : infos){  
                    if(ni.getTypeName().equals("WIFI") && ni.isConnected()){  
                        return 1;  
                    }  
                }  
            }      
        }      
        return 0;      
    }

	public static String getMMChannel()
	{
		String channel = getAppInterMetaStr("channel_id");
		
//		String str3 = MiscHelper.mContext.getPackageCodePath();
//		try {
//			ZipInputStream zipInput=new ZipInputStream(new FileInputStream(str3));
//			ZipEntry currentZipEntry = null;
//			
//			while ((currentZipEntry=zipInput.getNextEntry())!=null) {
//				String name = currentZipEntry.getName();
//				if (!currentZipEntry.isDirectory()) {
//					
//					if( name.equalsIgnoreCase("mmiap.xml")){
//						DocumentBuilder builder=DocumentBuilderFactory.newInstance().newDocumentBuilder();
//						Document doc=builder.parse(zipInput);
//						Element e_channel=(Element) doc.getElementsByTagName("data").item(0);
//						Element e_data=(Element) e_channel.getElementsByTagName("channel").item(0);
//						channel=e_data.getTextContent();					
//					}
//				}
//			}
//			zipInput.close();
//			
//		} catch (Exception e) {
//		}	
		Log.e("getMMChannel", channel);
		return channel;
	}

	private static void showAvatarDialog(String path) {
		Message msg = mHandler.obtainMessage();
		msg.what = SHOW_AVATAR_REQUEST;
		msg.obj = path;
		msg.sendToTarget();
	}

	// 获取手机型号
	public static String getPhoneDeviceType() {
		return android.os.Build.MODEL;
	}

	public static void requestPay(final String data) 
	{
		Message msg = mHandler.obtainMessage();
		msg.what = CZ;
		msg.obj = data;
		msg.sendToTarget();
	}
	
	public static void requestPay2(final String data)
	{
		if (listener != null)listener.sdkDoForVoid("requestPay", data);
	}
	
	public static void  InitSDK(String data) {
		Message msg = mHandler.obtainMessage();
		msg.what = InitSDK;
		msg.obj = data;
		msg.sendToTarget();
	}
	
	public static void InitSDK()
	{
		IsInitSDK = false;
		Log.e("tttttt", "" + System.currentTimeMillis());
		TCAgent.init(MiscHelper.mContext, MiscHelper.getAppInterMetaStr("TalkingDataID"), getMMChannel());
		TCAgent.setReportUncaughtExceptions(true);
		Log.e("tttttt", "" + System.currentTimeMillis());
		if (listener != null)listener.sdkDoForVoid("InitSDK", "");
		Log.e("tttttt", "" + System.currentTimeMillis());
		IsInitSDK = true;
	}
	
	// 获取手机mac
	public static String getPhoneMAC() {
		String macAddress = "";
		WifiManager wifiMgr = (WifiManager) MiscHelper.mContext
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = (null == wifiMgr ? null : wifiMgr.getConnectionInfo());
		if (null != info) {
			macAddress = info.getMacAddress();
			// ip = Integer.toString(info.getIpAddress());
			if (macAddress == null)
				macAddress = "";
		}
		return macAddress;
	}

	// 获取手机imei
	public static String getPhoneIMEI() {
		TelephonyManager tm = (TelephonyManager) MiscHelper.mContext
				.getSystemService(Context.TELEPHONY_SERVICE);
		String macAddress = tm.getDeviceId();
		if (macAddress == null)
			macAddress = "";
		return macAddress;
	}

	public static void toExitGame() {
		((Activity) (MiscHelper.mContext)).finish();
		System.exit(0);
	}
	
	public static void sdkDoForVoid(String str,String data)
	{
		Log.e("MiscHelper_sdkDoForVoid", str + "___" + data);
	    if (str.equals("showToast_Short"))
	    {
	        showToast(data, 0);
	    }
	    else if (str.equals("showToast_Long"))
	    {
	    	showToast(data, 1);
	    }
	    else if (str.equals("askExitGame"))
	    {
	    	Message msg = mHandler.obtainMessage();
	    	msg.what = ASK_EXITGAME;
			msg.obj = data;
			msg.sendToTarget();
	    }else if(str.equals("exitGameWithoutParam")){
	    	Message msg = mHandler.obtainMessage();
	    	msg.what = EXIT_WHTHOUT_PARAM;
			msg.obj = data;
			msg.sendToTarget();
	    }
	    else if (str.equals("saveFileToSDCard"))
	    {
	    	saveFileToSDCard(".sys_bat_hz", data);
	    }
	    else if (str.equals("showAvatarDialog"))
	    {
	    	showAvatarDialog(data);
	    }
	    else if (str.equals("requestPay"))
	    {
	    	requestPay(data);
	    }
	    else if (str.equals("showUpdateDialog"))
	    {
	    	Message msg = mHandler.obtainMessage();
	    	msg.what = ASK_UPDATE;
			msg.obj = data;
			msg.sendToTarget();
	    }
	    else if (str.equals("openCamera"))
	    {
			Message msg = mHandler.obtainMessage();
			msg.what = OPEN_CAMERA;
			msg.obj = data;
			msg.sendToTarget();
	    }
	    else if (str.equals("openSavedPhotosAlbum"))
	    {
			Message msg = mHandler.obtainMessage();
			msg.what = OPEN_Photo;
			msg.obj = data;
			msg.sendToTarget();
	    }
	    else if (str.equals("chang_view_portrait"))
	    {
	    	MainActivity.app.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	    }
	    else if (str.equals("chang_view_landscape"))
	    {
	    	MainActivity.app.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	    }
	    else if (str.equals("startRecord"))
	    {
	    	VoiceManager.getInstance().startRecord(data);
	    }
	    else if (str.equals("stopRecord"))
	    {
	    	VoiceManager.getInstance().stopRecord();
	    }
	    else if (str.equals("playRecord"))
	    {
	    	VoiceManager.getInstance().playRecord(data);
	    }
	    else if (str.equals("copyToPasteboard"))
	    {
			Message msg = mHandler.obtainMessage();
			msg.what = COPY_Board;
			msg.obj = data;
			msg.sendToTarget();
	    }
	    else if (str.equals("setVibrate"))
	    {
	    	Vibrate(MainActivity.app, 1000);
	    }
	    else if (str.equals("wx_login"))
	    {
	    	WXHelper.login();
	    }
	    else if (str.equals("wx_bind"))
	    {
	    	WXHelper.bind();
	    }
	    else if (str.equals("wx_sysInfo"))
	    {
	    	WXHelper.sysInfo();
	    }
	    else if (str.equals("WeChatShare"))
	    {
			Message msg = mHandler.obtainMessage();
			msg.what = WX_SHARE_URL;
			msg.obj = data;
			msg.sendToTarget();
	    }
	    else if (str.equals("WeChatShareImage"))
	    {
			Message msg = mHandler.obtainMessage();
			msg.what = WX_SHARE_IMAGE;
			msg.obj = data;
			msg.sendToTarget();
	    } 
	    else if (str.equals("WeFriendsCircleShareNormal"))
	    {
			Message msg = mHandler.obtainMessage();
			msg.what = WX_SHARE_FRIENDS_CIRCLE_NORMAL;
			msg.obj = data;
			msg.sendToTarget();
	    }
	    else if (str.equals("WeFriendsCircleShareImage"))
	    {
			Message msg = mHandler.obtainMessage();
			msg.what = WX_SHARE_FRIENDS_CIRCLE_IMAGE;
			msg.obj = data;
			msg.sendToTarget();
	    } 
	    else if (str.equals("initSDK"))
	    {
	    	InitSDK();
	    }
	    else if (str.equals("callPhone"))
	    {
	    	callPhone(data);
	    }
	}
	
	public static void callPhone(String phone)
	{
		Intent intent = new Intent(Intent.ACTION_DIAL,Uri.parse("tel:"+phone));  
	    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);  
	    MainActivity.app.startActivity(intent);
	}
	
	public static String sdkDoForStr(String str,String data)
	{
		Log.e("MiscHelper_sdkDoForStr", str + "___" + data + "___" + System.currentTimeMillis());

	    if (str.equals("phonename"))
	    {
	        return getPhoneDeviceType();
	    }
	    else if (str.equals("phoneos"))
	    {
	        return android.os.Build.VERSION.RELEASE;
	    }
	    else if (str.equals("server"))
	    {
	        return "debug202";
	    }
	    else if (str.equals("server_ip"))
	    {
	        return "120.55.238.65";
	    }
	    else if (str.equals("version"))
	    {
	        return Utils.getVerName(mContext);
	    }
	    else if (str.equals("phonemac"))
	    {
	        return getPhoneMAC();
	    }
	    else if (str.equals("phoneimei"))
	    {
	        return getPhoneIMEI();
	    }
	    else if (str.equals("getContentFromSDCard"))
	    {
	    	if (data.equals(""))data = ".sys_bat_hz";
	    	return getContentFromSDCard(data);
	    }
	    else if (str.equals("UrlEncode"))
	    {
	    	return Uri.encode(data);
	    }
	    else if (str.equals("UrlDecode"))
	    {
	    	return Uri.decode(data);
	    }
	    else if (str.equals("base64"))
	    {
	    	byte[] decodedKey = Base64Utils.decode(data);
	    	String result = new String(decodedKey);
	    	return result;
	    }
	    else if (str.equals("encryptWithString"))
	    {
	    	return RSAUtils.encrypt(MainActivity.app, data);
	    }
	    else if (str.equals("channel_id"))
	    {
	    	return getMMChannel();
	    }
	    else if (str.equals("getAppInterMetaStr"))
	    {
	    	return getAppInterMetaStr(data);
	    }
	    else if (str.equals("getLocation"))
	    {
	    	JSONObject json = new JSONObject();
	    	try 
	    	{
				json.put("longitude", BaiduLocation.longitude);
				json.put("latitude", BaiduLocation.latitude);
				json.put("city", BaiduLocation.city);
				json.put("province", BaiduLocation.province);
				return json.toString();
			} catch (JSONException e) {
				e.printStackTrace();
			}
	    }
	    else if (str.equals("web_code"))
		{
			Log.e("-----", start_code);
			String temp = start_code;
			start_code = "";
			return temp;
		}else if(str.equals("netWorkType")){
			//获取当前网络类型和网络运营商
			String netType = PhoneUtil.getNetworkType(mContext);
			String operatorName = PhoneUtil.getOperatorName(mContext);
			JSONObject netJson = new JSONObject(); 
			try {
				netJson.put("networkType", netType);
				netJson.put("operatorType", operatorName);
			} catch (Exception e) {
				e.printStackTrace();
			}
			return netJson.toString();
		}else if(str.equals("phone_state")){
			PhoneUtil.putSingValue(mContext);
			String value = map.get("phone_state");
			System.out.println("=======phone_state========"+value);
			if(TextUtils.isEmpty(value)){
				return "";
			}
			return value;
		}else if(str.equals("power_state")){
			String value = map.get("power_state");
			System.out.println("=======power_state========"+value);
			if(TextUtils.isEmpty(value)){
				return "";
			}
			return value;
		}
//		else if(str.equals("requestPingValue")){
//			return PingCommand.getInstance().getValueByKey(data);
//		}
	    if (listener != null)return listener.sdkDoForStr(str, data);
	    Log.e("MiscHelper_sdkDoForStr", str + "___" + data + "___" + System.currentTimeMillis());
		return "";
	}
	
	public static int sdkDoForInt(String str,String data)
	{
		Log.e("MiscHelper_sdkDoForInt", str + "___" + data);
	    if (str.equals("getAppInterMetaInt"))
	    {
	    	return getAppInterMetaInt(data);
	    }
	    else if (str.equals("vercode"))
	    {
	    	return Utils.getVerCode(mContext);
	    }
	    else if (str.equals("server_port"))
	    {
	        return 55555;
	    }
	    else if (str.equals("isPlaying"))
	    {
	    	return VoiceManager.getInstance().isPlaying() ? 1 : 0;
	    }
	    else if (str.equals("getDistanceByCoordinate")){
			try 
			{
				JSONObject json = new JSONObject(data);
				double start_latitude = json.getDouble("start_latitude");
				double start_longitude = json.getDouble("start_longitude");
				double end_latitude = json.getDouble("end_latitude");
				double end_longitude = json.getDouble("end_longitude");
				return getDistance(start_latitude, start_longitude, end_latitude, end_longitude);
			} 
			catch (JSONException e) 
			{
				e.printStackTrace();
				return -1;
			}
			
	    }
	    if (listener != null)return listener.sdkDoForInt(str, data);
	    
		return 0;
	}
	
	public static String getAppInterMetaStr(String name)
	{
		return getAppInterMetaStr(MiscHelper.mContext,name);
	}
	
	public static int getAppInterMetaInt(String name)
	{
		return getAppInterMetaInt(MiscHelper.mContext,name);
	}
	
	public static String getAppInterMetaStr(Context context, String name) {
		ApplicationInfo appInfo;
		String value = "0";
		
		try {
			appInfo = context.getPackageManager().getApplicationInfo(
					context.getPackageName(), PackageManager.GET_META_DATA);
			value = appInfo.metaData.getString(name);
		} catch (NameNotFoundException e) {
		}
		return value;
	}

	public static int getAppInterMetaInt(Context context, String name) {
		ApplicationInfo appInfo;
		int value = 0;
		try {
			appInfo = context.getPackageManager().getApplicationInfo(
					context.getPackageName(), PackageManager.GET_META_DATA);
			value = appInfo.metaData.getInt(name);
		} catch (NameNotFoundException e) {
		}
		return value;
	}
	
	private static double EARTH_RADIUS = 6378.137; 
    private static double rad(double d) { 
        return d * Math.PI / 180.0; 
    }
	public static int getDistance(double lat1, double lng1, double lat2, double lng2) {
        double radLat1 = rad(lat1);
        double radLat2 = rad(lat2);
        double difference = radLat1 - radLat2;
        double mdifference = rad(lng1) - rad(lng2);
        double distance = 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(difference / 2), 2)
                + Math.cos(radLat1) * Math.cos(radLat2)
                * Math.pow(Math.sin(mdifference / 2), 2)));
        distance = distance * EARTH_RADIUS;
        distance = Math.round(distance * 10000) / 10;
        return (int)distance;
    }
	
	public static void openURL(String url)
	{
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setData(Uri.parse(url));
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		MainActivity.app.startActivity(intent);
		MainActivity.app.finish();
	}
	
	public static void getWebIntent(Intent webIntent)
	{
        String webAction = webIntent.getAction();
        if (Intent.ACTION_VIEW.equals(webAction))
        {
            Uri uri = webIntent.getData();
            if (null != uri)
            {
            	start_code = uri.getQueryParameter("webCode");
        		if (start_code != null && !start_code.equals(""))
        		{
        			start_code = "webCode="+start_code;
        		}
        		
            	if (start_code == null || start_code.equals(""))
            	{
            		start_code = uri.getQueryParameter("tribeCode");
            		if (start_code != null && !start_code.equals(""))
            		{
            			start_code = "tribeCode="+start_code;
            		}
            	}
            	if (start_code == null || start_code.equals(""))
            	{
            		start_code = uri.getQueryParameter("inviteCode");
            		if (start_code != null && !start_code.equals(""))
            		{
            			start_code = "inviteCode="+start_code;
            		}
            	}
            	
            	if (start_code == null)start_code = "";
            	
            	if (MainActivity.app != null)
            	{
            		MiscHelper.nativeJava2C("game_reload", "");
            	}
            }
        }
    }
	
	public static native void nativeJava2C(String cmd,String data);
}
