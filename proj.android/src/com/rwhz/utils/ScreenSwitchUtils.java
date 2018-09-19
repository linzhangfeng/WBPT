package com.rwhz.utils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.view.OrientationEventListener;

public class ScreenSwitchUtils {
	
	 private Activity activity;
	 
	 public static ScreenSwitchUtils screenUtils;
	 
	 private SreenOrientationListener listenner;
	 
	 private ScreenSwitchUtils(Activity context){
		 this.activity = context;
	 }
	 
	 public static ScreenSwitchUtils getInstance(Activity context){
		 if(screenUtils == null){
			 screenUtils = new ScreenSwitchUtils(context);
		 }
		 return screenUtils;
	 }
	 
	 public void enable(){
		 if(listenner == null){
			 listenner = new SreenOrientationListener(activity);
		 }
		 listenner.enable();
	 }
	 
	 public void disable(){
		 if(listenner != null){
			 listenner.disable();
		 }
	 }
	
	 class SreenOrientationListener extends OrientationEventListener {

	        public SreenOrientationListener(Context context) {
	            super(context);
	        }

	        @Override
	        public void onOrientationChanged(int orientation) {
	            if (orientation == OrientationEventListener.ORIENTATION_UNKNOWN) {
	                return; // 手机平放时，检测不到有效的角度
	            }
	            // 只检测是否有四个角度的改变
	            if (orientation > 350 || orientation < 10) {
	                // 0度：手机默认竖屏状态（home键在正下方）
	                orientation = 0;
//	                System.out.println("=========0度：手机默认竖屏状态（home键在正下方=======");
	            } else if (orientation > 80 && orientation < 100) {
	                // 90度：手机顺时针旋转90度横屏（home建在左侧）
//	            	System.out.println("=========90度：手机顺时针旋转90度横屏（home建在左侧）=======");
	            	//横屏反方向
	            	if (activity.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE)
	            	{
	            	   activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
	            	}
	            } else if (orientation > 170 && orientation < 190) {
	                // 手机顺时针旋转180度竖屏（home键在上方）
	                orientation = 180;
//	                System.out.println("=========手机顺时针旋转180度竖屏（home键在上方）=======");
	            } else if (orientation > 260 && orientation < 280) {
	                // 手机顺时针旋转270度横屏，（home键在右侧）
//	            	System.out.println("=========手机顺时针旋转270度横屏，（home键在右侧）=======");
	            	if(activity.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE)
	            	{
	            	   activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	            	}
	            }
	        }
	    }
	
}
