package com.rwhz.app;

import android.content.Intent;

public interface MainListener {
	
	public String sdkDoForStr(String str,String data);
	
	public void sdkDoForVoid(String str,String data);
	
	public int sdkDoForInt(String str,String data);
	
	public void main_onDestroy();
	
	public void main_onResume();
	
	public void main_onPause();
	
	public void onMyActivityResult(int requestCode, int resultCode,
			final Intent data);
}
