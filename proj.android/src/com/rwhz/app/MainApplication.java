package com.rwhz.app;

import android.app.Application;
import android.content.Context;

public class MainApplication extends Application {

//	@Override
	// 移动mm 新加  start
	protected void attachBaseContext(Context ctx){
	    super.attachBaseContext(ctx);
	   
	 }
	// 移动mm 新加  end
	@Override
	public void onCreate() {
		super.onCreate();
		
	}
	
}
