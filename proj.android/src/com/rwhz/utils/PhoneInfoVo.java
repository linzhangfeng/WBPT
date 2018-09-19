package com.rwhz.utils;

public class PhoneInfoVo {
	
	private String imsi;  //手机imsi
	private String imei;  //手机imei
	private String phoneModel; //手机型号
	private String line1Number;  //手机号码
	private String simSerialNumber;  //手机sim 序列号
	private boolean simStatusReady =false; //sim卡状态是否良好(无卡,未知状态,需要NetworkPIN解锁,需要PIN解锁,需要PUK解锁)
	private int spType = -1;
	
	
	public String getImsi() {
		return imsi;
	}
	public void setImsi(String imsi) {
		this.imsi = imsi;
	}
	public String getImei() {
		return imei;
	}
	public void setImei(String imei) {
		this.imei = imei;
	}	
	
	public String getPhoneModel() {
		if(phoneModel != null && !phoneModel.equals("") && phoneModel.length() >100)
			phoneModel.substring(0, 100);
		return phoneModel;
	}
	
	public void setPhoneModel(String phoneModel) {
		this.phoneModel = phoneModel;
	}
	
	public String getLine1Number() {
		return line1Number;
	}
	
	public void setLine1Number(String line1Number) {
		this.line1Number = line1Number;
	}
	
	public String getSimSerialNumber() {
		return simSerialNumber;
	}
	
	public void setSimSerialNumber(String simSerialNumber) {
		this.simSerialNumber = simSerialNumber;
	}
	public int getSpType() {
		if(imsi != null && !imsi.equals("")){
			if(simStatusReady){
    			if (imsi.startsWith("46000") || imsi.startsWith("46002") || imsi.startsWith("46007")) {
    				// 因为移动网络编号46000下的IMSI已经用完，所以虚拟了一个46002编号，134/159号段使用了此编号
    				spType = 1;// 中国移动
    			} else if (imsi.startsWith("46001") || imsi.startsWith("46006")) {
    				spType = 2;// 中国联通
    			} else if (imsi.startsWith("46003") || imsi.startsWith("46005")) {
    				spType = 3;// 中国电信
    			}
    		}
		}
		//spType=1;
		return spType;
	}
	
	public void setSpType(int spType) {
		this.spType = spType;
	}
	public boolean isSimStatusReady() {
		return simStatusReady;
	}
	public void setSimStatusReady(boolean simStatusReady) {
		this.simStatusReady = simStatusReady;
	}
}
