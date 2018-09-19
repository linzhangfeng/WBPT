package com.rwhz.wx;

import com.rwhz.jni.MiscHelper;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPI;

public class WXHelper 
{
	public static IWXAPI api;
	public static boolean isLogin = false;
	public static boolean isUse = false;
	public static boolean checkWXEnable()
	{
		if (api.isWXAppInstalled() == false)
		{
			MiscHelper.showToast("请先安装微信", 0);
		}
		return true;
	}
	
	public static void login()
	{
		if (checkWXEnable() == false)
		{
			return;
		}
		final SendAuth.Req req = new SendAuth.Req();
	    req.scope = "snsapi_userinfo";
	    req.state = "wx_login_code";
	    api.sendReq(req);
	    
	    isLogin = true;
	    isUse = true;
	}
	
	public static void bind()
	{
		if (checkWXEnable() == false)
		{
			return;
		}
		final SendAuth.Req req = new SendAuth.Req();
	    req.scope = "snsapi_userinfo";
	    req.state = "wx_authorize";
	    api.sendReq(req);
	    
	    isLogin = false;
	    isUse = true;
	}
	
	public static void sysInfo()
	{
		if (checkWXEnable() == false)
		{
			return;
		}
		final SendAuth.Req req = new SendAuth.Req();
	    req.scope = "snsapi_userinfo";
	    req.state = "wx_sysInfo";
	    api.sendReq(req);
	    
	    isLogin = false;
	    isUse = true;
	}
}
