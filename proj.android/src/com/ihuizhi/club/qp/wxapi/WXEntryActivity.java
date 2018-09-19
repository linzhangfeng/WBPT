package com.ihuizhi.club.qp.wxapi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.rwhz.jni.MiscHelper;
import com.rwhz.wx.WXHelper;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler{
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);		
		WXHelper.api.handleIntent(getIntent(), this);
	}
	
	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		
		setIntent(intent);
        WXHelper.api.handleIntent(intent, this);
	}
	
	@Override
	public void onReq(BaseReq arg0) {
	}

	@Override
	public void onResp(BaseResp resp) 
	{
		Log.e("WXEntryActivity", "onResp" + resp.errCode);
		if (resp.errCode == BaseResp.ErrCode.ERR_OK)
		{
			if (WXHelper.isUse)
			{
				SendAuth.Resp r = (SendAuth.Resp)resp;
				String code = r.code;
				String state = r.state;
				if (state.equals("wx_login_code"))
				{
					Log.e("WXEntryActivity", "wx_login_code" + code);
					MiscHelper.nativeJava2C("wx_login_code", code + "");
				}
				else if (state.equals("wx_authorize"))
				{
					Log.e("WXEntryActivity", "wx_authorize" + code);
					MiscHelper.nativeJava2C("wx_authorize", code);
				}
			}

		}
		WXHelper.isUse = false;
		finish();
	}
}
