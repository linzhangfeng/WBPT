package com.rwhz.baidu;

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.util.Log;

import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;

/**
 * 百度提供定位方法 定位成功之后缓存数据到MConfig
 * 每隔5分钟重启一次定位程序
 * @author xiongzheng
 * 
 */
public class BaiduLocation {

	private static Object lock = new Object();
	private static BaiduLocation baiduLocation;

	private LocationClient mLocClient;
	private HuizhiLocationListenner locationListener = new HuizhiLocationListenner();

	public static double longitude;
	public static double latitude;
	public static String province;
	public static String city;
	private Timer timer;
	private Timer timeLocationManager;//重启任务
	private BaiduLocation(Context context) {
		init(context);
	}

	public static BaiduLocation getInstance(Context context) {
		if (baiduLocation != null) {
			return baiduLocation;
		}
		synchronized (lock) {
			if (baiduLocation == null) {
				baiduLocation = new BaiduLocation(context);
			}
		}
		return baiduLocation;
	}

	private void init(Context context) {
		mLocClient = new LocationClient(context);
		LocationClientOption option = new LocationClientOption();
		option.setLocationMode(LocationClientOption.LocationMode.Hight_Accuracy);
		option.setIsNeedAddress(true);
		option.setOpenGps(true); // 打开gps
		option.setCoorType("bd09ll"); // 设置坐标类型
		option.setScanSpan(1000);
		option.setAddrType("all");
		mLocClient.setLocOption(option);
	}

	public void location() {
//		mLocClient.requestLocation();
		mLocClient.registerLocationListener(locationListener);
		mLocClient.start();
	}

	/**
	 * 定位SDK监听函数
	 */
	public class HuizhiLocationListenner implements BDLocationListener {

		@Override
		public void onReceiveLocation(BDLocation location) {
			try {
				// map view 销毁后不再处理新接收的位置
				if (location == null) {
					return;
				}
				
				longitude = location.getLongitude();
				latitude = location.getLatitude();
				city = location.getCity();
				province = location.getProvince();
				Log.e("longitude", longitude + "");
				Log.e("latitude", latitude + "");
//				
				if(timer == null){
					timer = new Timer();
					timer.schedule(new CloseTask(), 20000);
				}
				
            } catch (Exception e) {
            	e.printStackTrace();
            }			
		}

		public void onReceivePoi(BDLocation poiLocation) {

		}
	}
	
	private class CloseTask extends TimerTask{

		@Override
		public void run() {
			Log.e("BaiduLocation","关闭GPS");
			mLocClient.unRegisterLocationListener(locationListener);
			mLocClient.stop();
			timer = null;
			//关闭的时候启动下次GPS定位倒计时5分钟
			if(timeLocationManager == null){
				Log.e("BaiduLocation","准备下一次定位开始");
				timeLocationManager = new Timer();
				timeLocationManager.schedule(new ManagerTask(), 5 * 60 * 1000);
			}
		}
	}
	
	private class ManagerTask extends TimerTask{
		@Override
		public void run() {
			timeLocationManager = null;
			Log.e("BaiduLocation","重启定位程序");
			LocationClientOption option = new LocationClientOption();
			option.setLocationMode(LocationClientOption.LocationMode.Hight_Accuracy);
			option.setIsNeedAddress(true);
			option.setOpenGps(true); // 打开gps
			option.setCoorType("bd09ll"); // 设置坐标类型
			option.setScanSpan(1000);
			option.setAddrType("all");
			mLocClient.setLocOption(option);
			location();
		}
	}
	
}
