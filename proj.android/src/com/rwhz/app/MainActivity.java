/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.rwhz.app;

import java.io.File;
import java.io.FileOutputStream;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.support.v4.app.NotificationCompat;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
import android.text.ClipboardManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

import com.ihuizhi.club.qp.R;
import com.ihuizhi.transferframwork.TransferFramworkManager;
import com.ihuizhi.transferframwork.api.base.CommonResult;
import com.ihuizhi.transferframwork.db.FramworkDBUtil;
import com.ihuizhi.transferframwork.net.base.ProtocolType;
import com.ihuizhi.transferframwork.transfer.ITransferCallback;
import com.ihuizhi.transferframwork.transfer.TransferTask;
import com.ihuizhi.transferframwork.utils.BeanUtils;
import com.rwhz.baidu.BaiduLocation;
import com.rwhz.jni.MiscHelper;
import com.rwhz.upgrade.CheckUpgradeDialog;
import com.rwhz.upgrade.CheckUpgradeDialog.OnUpgradeListener;
import com.rwhz.utils.BatteryReceiver;
import com.rwhz.utils.PhoneSigLevelReceiver;
import com.rwhz.utils.PhoneUtil;
import com.rwhz.utils.ScreenSwitchUtils;
import com.rwhz.utils.ShareMgr;
import com.rwhz.utils.Utils;
import com.rwhz.wx.WXHelper;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class MainActivity extends Cocos2dxActivity {
	private static final String tag = "ZJH";
	public static MainActivity app = null;
	//订正微信邀请keyback的时候弹出退出应用或者退出游戏的对话框
	private boolean isOnTop = true;
	private BatteryReceiver batteryReceiver = new BatteryReceiver();
	private PhoneSigLevelReceiver phoneStateReceiver;
	
	private ScreenSwitchUtils sreenUtil;
	
	public void initSDK()
	{
		ShareMgr.init(getApplicationContext());
		WXHelper.api = WXAPIFactory.createWXAPI(this, "wxf9f89e76d7b349e9",
		false);
		WXHelper.api.registerApp("wxf9f89e76d7b349e9");
		
		BaiduLocation.getInstance(this).location();
		TransferFramworkManager.getInstance().init(this, null);
		//注册电量监听
		IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		registerReceiver(batteryReceiver, intentFilter);
		//注册手机信号强度监听
		TelephonyManager  mTelephonyManager = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);//获取TelephonyManager 对象。
		phoneStateReceiver = new PhoneSigLevelReceiver(getContext());
		mTelephonyManager.listen(phoneStateReceiver,PhoneStateListener.LISTEN_SIGNAL_STRENGTHS);
	}
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		app = this;
		MiscHelper.init(mHandler, this);
		setKeepScreenOn(true);
		MainActivity.app
				.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		checkApp();
		Log.e(tag, "MainActivity====onCreate===" + System.currentTimeMillis());
		sreenUtil = ScreenSwitchUtils.getInstance(this);
		initSDK();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		TransferFramworkManager.getInstance().destroy(this);
		unregisterReceiver(batteryReceiver);
		if (MiscHelper.listener != null)
			MiscHelper.listener.main_onDestroy();
	}

	protected void onResume() {
		super.onResume();
		if (MiscHelper.listener != null)
			MiscHelper.listener.main_onResume();
		sreenUtil.enable();
		//避免dispatchKeyEvent调用在onResume（）之后，isOnTop修改太早
		new Handler().postDelayed(new Runnable(){
			@Override
			public void run() {
				isOnTop = true;
			}
		}, 300);
		if(dialog != null && dialog.isShowing()){
			dialog.dismiss();
			dialog.show();
		}
		Log.e(tag, "MainActivity====onResume===");
	}
	private CheckUpgradeDialog dialog;
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		//切换为竖屏
		if (newConfig.orientation == this.getResources().getConfiguration().ORIENTATION_PORTRAIT) {
			if(dialog != null && dialog.isShowing()){
				dialog.refreshUI();
			}
		}
		//切换为横屏
		else if (newConfig.orientation == this.getResources().getConfiguration().ORIENTATION_LANDSCAPE) {
			if(dialog != null && dialog.isShowing()){
				dialog.refreshUI();
			}
		}
	}
	
	protected void onPause() {
		super.onPause();
		if (MiscHelper.listener != null)
			MiscHelper.listener.main_onPause();
		isOnTop = false;
		sreenUtil.disable();
		Log.e(tag, "MainActivity====onPause===");
	}

	private static Toast toast;
	private static Toast toast2;

	public static void toastLong(String show) {
		if (toast != null) {
			toast.setText(show);
			toast.setDuration(Toast.LENGTH_LONG);
			toast.show();
			return;
		}
		toast = Toast.makeText(MainActivity.app, show, Toast.LENGTH_LONG);
		toast.show();
	}

	public static void toastShort(String show) {
		if (toast2 != null) {
			toast2.setText(show);
			toast2.setDuration(Toast.LENGTH_SHORT);
			toast2.show();
			return;
		}
		toast2 = Toast.makeText(MainActivity.app, show, Toast.LENGTH_SHORT);
		toast2.show();
	}

	private Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			// 微信 sdk 对接
			case MiscHelper.InitSDK:
				MiscHelper.InitSDK();
				break;

			case MiscHelper.CZ:
				MiscHelper.requestPay2((String) msg.obj);
				break;

			case MiscHelper.VIBRATE_REQUEST:
				MiscHelper.Vibrate(MainActivity.this, 1000 * 2);
				break;
			case MiscHelper.TOAST_LONG_REQUEST:
				MainActivity.toastLong(String.valueOf(msg.obj));
				break;
			case MiscHelper.TOAST_SHORT_REQUEST:
				MainActivity.toastShort(String.valueOf(msg.obj));
				break;

			case MiscHelper.SHOW_AVATAR_REQUEST:
				String savePath = (String) msg.obj;
				MainActivity.temp_image_path = savePath;
				showAvatarDialog();
				break;

			case MiscHelper.OPEN_Photo:
				MainActivity.temp_image_path = Environment
						.getExternalStorageDirectory()
						+ "/"
						+ System.currentTimeMillis() + ".png";// (String)
																// msg.obj;
				showPhoto();
				break;

			case MiscHelper.OPEN_CAMERA:
				MainActivity.temp_image_path = Environment
						.getExternalStorageDirectory()
						+ "/"
						+ System.currentTimeMillis() + ".png";// (String)
																// msg.obj;
				showCamera();
				break;

			case MiscHelper.COPY_Board:
				ClipboardManager myClipboard;
				myClipboard = (ClipboardManager) app
						.getSystemService(Context.CLIPBOARD_SERVICE);
				myClipboard.setText((String) msg.obj);
				break;

			case MiscHelper.WX_SHARE_URL:
				JSONObject json;
				try {
					json = new JSONObject((String) msg.obj);
					String title = json.getString("title");
					String text = json.getString("text");
					String imageUrl = json.getString("imageUrl");
					String url = json.getString("url");
					ShareMgr.shareToWX(title, text, imageUrl, url);
				} catch (JSONException e) {
					e.printStackTrace();
				}
				break;

			case MiscHelper.WX_SHARE_IMAGE:
				JSONObject json2;
				try {
					json2 = new JSONObject((String) msg.obj);
					String title = json2.getString("title");
					String imagePath = json2.getString("imagePath");
					ShareMgr.shareToWXImage(title, imagePath);
				} catch (JSONException e) {
					e.printStackTrace();
				}
				break;
			case MiscHelper.WX_SHARE_FRIENDS_CIRCLE_IMAGE:
				JSONObject jsonFImage;
				try {
					jsonFImage = new JSONObject((String) msg.obj);
					String title = jsonFImage.getString("title");
					String imagePath = jsonFImage.getString("imagePath");
					ShareMgr.shareToWXFriendsCircleImage(title, imagePath);
				} catch (JSONException e) {
					e.printStackTrace();
				}
			    break;
			case MiscHelper.WX_SHARE_FRIENDS_CIRCLE_NORMAL:
				JSONObject jsonNormal;
				try {
					jsonNormal = new JSONObject((String) msg.obj);
					String title = jsonNormal.getString("title");
					String text = jsonNormal.getString("text");
					String imageUrl = jsonNormal.getString("imageUrl");
					String url = jsonNormal.getString("url");
					ShareMgr.shareToWXZone(title, text, imageUrl, url);
				} catch (JSONException e) {
					e.printStackTrace();
				}
			    break;
			case MiscHelper.ASK_UPDATE:
				try {
					JSONObject json3 = new JSONObject((String) msg.obj);
					String desc = json3.getString("desc");
					final int isForce = json3.getInt("force");
					final String url = json3.getString("url");
					final String name = json3.getString("name");
					final double size = json3.getDouble("size");
					final String code = json3.getString("code");
//					 new AlertDialog.Builder(MainActivity.this)
//					 .setCancelable(false)
//					 .setTitle("是否更新")
//					 .setMessage(desc)
//					 .setPositiveButton("更新",
//					 new DialogInterface.OnClickListener() {
//					
//					 @Override
//					 public void onClick(
//					 DialogInterface mdialog, int which) {
//					 mdialog.dismiss();
//					 MiscHelper.openURL(url);
//					 System.exit(0);
//					 }
//					 })
//					 .setNegativeButton("取消",
//					 new DialogInterface.OnClickListener() {
//					
//					 @Override
//					 public void onClick(
//					 DialogInterface mdialog, int which) {
//					 mdialog.dismiss();
//					 if (isForce == 0)
//					 {
//					 MainActivity.this.finish();
//					 System.exit(0);
//					 }
//					 }
//					 }).show();
					doUpdate(isForce, name, desc, url, size, code);
//					GameUserMapDialog dialog = new GameUserMapDialog(MainActivity.this);
//					dialog.show();
				} catch (JSONException e1) {

					e1.printStackTrace();
				}

				break;

			case MiscHelper.ASK_EXITGAME:
				new AlertDialog.Builder(MainActivity.this)
						.setCancelable(false)
						.setMessage("是否退出游戏?")
						.setPositiveButton("退出",
								new DialogInterface.OnClickListener() {

									@Override
									public void onClick(
											DialogInterface mdialog, int which) {
										mdialog.dismiss();
										MainActivity.this.finish();
										System.exit(0);
									}
								})
						.setNegativeButton("取消",
								new DialogInterface.OnClickListener() {

									@Override
									public void onClick(
											DialogInterface mdialog, int which) {
										mdialog.dismiss();
									}
								}).show();
				break;
			case MiscHelper.EXIT_WHTHOUT_PARAM:	
				MainActivity.this.finish();
				System.exit(0);
				break;

			case MiscHelper.Check_NetWork:
				if (!MiscHelper.isOpenNetwork()) {
					new AlertDialog.Builder(MainActivity.this)
							.setCancelable(false)
							.setTitle("没有可用的网络")
							.setMessage("是否对网络进行设置?")
							.setNegativeButton("是",
									new DialogInterface.OnClickListener() {
										@Override
										public void onClick(
												DialogInterface mdialog,
												int which) {
											mdialog.dismiss();
											Intent intent = null;

											try {
												intent = new Intent(
														android.provider.Settings.ACTION_SETTINGS);
												MainActivity.this
														.startActivity(intent);
											} catch (Exception e) {
												e.printStackTrace();
											}
										}
									})
							.setPositiveButton("否",
									new DialogInterface.OnClickListener() {
										@Override
										public void onClick(
												DialogInterface mdialog,
												int which) {
											mdialog.dismiss();
										}
									}).show();
				}
				break;
			}
		}
	};

	// 以下调用射相头开始
	private String[] items = new String[] { "选择本地图片", "拍照" };

	/* 头像名称 */
	private static final String IMAGE_FILE_NAME = "faceImage.jpg";
	private static String temp_image_path;
	/* 请求码 */
	private static final int IMAGE_REQUEST_CODE = 0;
	private static final int CAMERA_REQUEST_CODE = 1;
	private static final int RESULT_REQUEST_CODE = 2;
	public static final int CHONGZHI_REQUEST_CODE = 3;

	public void showPhoto() {
		Intent intentFromGallery = new Intent();
		intentFromGallery.setType("image/*"); // 设置文件类型
		intentFromGallery.setAction(Intent.ACTION_GET_CONTENT);
		startActivityForResult(intentFromGallery, IMAGE_REQUEST_CODE);
	}

	public void showCamera() {
		Intent intentFromCapture = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		if (hasSdcard()) {
			intentFromCapture.putExtra(MediaStore.EXTRA_OUTPUT, Uri
					.fromFile(new File(Environment
							.getExternalStorageDirectory(), IMAGE_FILE_NAME)));
		}

		startActivityForResult(intentFromCapture, CAMERA_REQUEST_CODE);
	}

	/**
	 * 显示选择对话框
	 */
	private void showAvatarDialog() {
		new AlertDialog.Builder(this)
				.setTitle("设置头像")
				.setItems(items, new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						switch (which) {
						case 0:
							Intent intentFromGallery = new Intent();
							intentFromGallery.setType("image/*"); // 设置文件类型
							intentFromGallery
									.setAction(Intent.ACTION_GET_CONTENT);
							startActivityForResult(intentFromGallery,
									IMAGE_REQUEST_CODE);
							break;
						case 1:

							Intent intentFromCapture = new Intent(
									MediaStore.ACTION_IMAGE_CAPTURE);
							// 判断存储卡是否可以用，可用进行存储
							if (hasSdcard()) {

								intentFromCapture.putExtra(
										MediaStore.EXTRA_OUTPUT,
										Uri.fromFile(new File(Environment
												.getExternalStorageDirectory(),
												IMAGE_FILE_NAME)));
							}

							startActivityForResult(intentFromCapture,
									CAMERA_REQUEST_CODE);
							break;
						}
					}
				})
				.setNegativeButton("取消", new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				}).show();

	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode,
			final Intent data) {
		// 结果码不等于取消时候
		if (resultCode != RESULT_CANCELED) {
			switch (requestCode) {
			case IMAGE_REQUEST_CODE:
				// startPhotoZoom(data.getData());
				new Thread(new Runnable() {
					public void run() {
						try {
							Thread.sleep(100);
							startPhotoZoom(data.getData());
						} catch (Exception e) {

						}
					}
				}).start();
				break;
			case CAMERA_REQUEST_CODE:
				if (hasSdcard()) {
					// File tempFile = new File(
					// Environment.getExternalStorageDirectory()
					// + "/" + IMAGE_FILE_NAME);
					// Toast.makeText(ZJH.this, tempFile.toString(),
					// Toast.LENGTH_LONG).show();
					// Log.d("xxx", tempFile.toString());
					// startPhotoZoom(Uri.fromFile(tempFile));
					new Thread(new Runnable() {
						public void run() {
							try {
								Thread.sleep(100);
								File tempFile = new File(
										Environment
												.getExternalStorageDirectory()
												+ "/" + IMAGE_FILE_NAME);
								// Log.d("xxx", tempFile.toString());
								startPhotoZoom(Uri.fromFile(tempFile));
							} catch (Exception e) {

							}
						}
					}).start();
				} else {
					Toast.makeText(MainActivity.this, "未找到存储卡，无法存储照片！",
							Toast.LENGTH_LONG).show();
				}

				break;
			case RESULT_REQUEST_CODE:
				if (data != null) {
					Log.d("xxx", "RESULT_REQUEST_CODE");
					getImageToView(data);
				}
				break;
			}
		}

		if (MiscHelper.listener != null)
			MiscHelper.listener.onMyActivityResult(requestCode, resultCode,
					data);
		super.onActivityResult(requestCode, resultCode, data);
	}

	/**
	 * 裁剪图片方法实现
	 * 
	 * @param uri
	 */
	public void startPhotoZoom(Uri uri) {
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, "image/*");
		// 设置裁剪
		intent.putExtra("crop", "true");
		// aspectX aspectY 是宽高的比例
		intent.putExtra("aspectX", 1);
		intent.putExtra("aspectY", 1);
		// outputX outputY 是裁剪图片宽高
		intent.putExtra("outputX", 300);
		intent.putExtra("outputY", 300);
		intent.putExtra("return-data", true);

		startActivityForResult(intent, RESULT_REQUEST_CODE);
	}

	/**
	 * 保存裁剪之后的图片数据
	 * 
	 * @param picdata
	 */
	private void getImageToView(Intent data) {
		Bundle extras = data.getExtras();
		if (extras != null) {
			final Bitmap photo = extras.getParcelable("data");
			Log.d("xxx", photo.toString());
			Runnable saveFileRunnable = new Runnable() {
				@Override
				public void run() {
					saveMyBitmap("texasImage", photo);
					MiscHelper.nativeJava2C("image_notify", temp_image_path);
				}
			};
			new Thread(saveFileRunnable).start();
		}
	}

	public void saveMyBitmap(String bitName, Bitmap photo) {

		String dir = MainActivity.temp_image_path.substring(0,
				MainActivity.temp_image_path.lastIndexOf("/"));

		String fileName = MainActivity.temp_image_path
				.substring(MainActivity.temp_image_path.lastIndexOf("/") + 1);
		File fileDir = new File(dir);
		if (!fileDir.exists())
			fileDir.mkdirs();

		File f = new File(fileDir, fileName);

		try {
			f.createNewFile();
		} catch (Exception e1) {
			e1.printStackTrace();
		}

		FileOutputStream fOut = null;
		try {
			fOut = new FileOutputStream(f);
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (photo != null && fOut != null) {
			photo.compress(Bitmap.CompressFormat.PNG, 100, fOut);
		}
		try {
			fOut.flush();
		} catch (Exception e) {
			e.printStackTrace();
		}
		try {
			fOut.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static boolean hasSdcard() {
		String state = Environment.getExternalStorageState();
		if (state.equals(Environment.MEDIA_MOUNTED)) {
			return true;
		} else {
			return false;
		}
	}

	public String getAppCacheDir() {
		return getCacheDir().toString();
	}

	private DownloadCallback mDownloadCallback;
	private static final int DOWNLOAD_START = 0;
	private static final int DOWNLOAD_RUN = 1;
	private static final int DOWNLOAD_END = 2;

	/**
	 * 处理升级逻辑 force 是否强制升级 url 下载地址
	 */
	public void doUpdate(final int force, String name, String desc,
			final String url, double size, final String code) {
		if(dialog != null){
			dialog.dismiss();
		}
		dialog = new CheckUpgradeDialog(this);
		System.out.println("========doupdate========"+dialog);
		dialog.setVersion(name);
		dialog.setSize(Utils.getFormatSize(size));
		dialog.setDesc(desc);
		final boolean isForce = force==0?true:false;
		dialog.setForce(isForce);
		dialog.setmOnUpgradeListener(new OnUpgradeListener() {
			@Override
			public void onPositiveClick() {
				// //下载
				String fileName = "iwanbei_" + code + "_upgrade.apk";
				final String path = Environment.getExternalStorageDirectory()
						+ File.separator + fileName;
				mDownloadCallback = new DownloadCallback(MainActivity.this,url, path, isForce);
				mDownloadCallback.setmDownloadListener(dialog);
				TransferTask task = FramworkDBUtil.getTransferTaskByMD5(MainActivity.this, BeanUtils.md532(url));
				System.err.println("task::::"+task);
				if (task != null){
					System.err.println("task.getSavepath()::::"+task.getSavepath());
					System.err.println("task.getStatus()::::"+task.getStatus());
				}
				TransferFramworkManager.getInstance().registerCallback(
						mDownloadCallback);
				TransferFramworkManager.getInstance().startDownload(fileName,
						url, path);
			}

			@Override
			public void onNegtiveClick() {
				if (isForce) {
					MainActivity.this.finish();
					System.exit(0);
				}
			}
		});
		dialog.show();
	}

	/**
	 * 处理下载通知栏 1.开始下载 2.正在下载 3.下载成功
	 */
	private void dealDownloadNotification(Context context, int code,
			int progress) {
		NotificationManager manager = (NotificationManager) context
				.getSystemService(Context.NOTIFICATION_SERVICE);
		switch (code) {
		case DOWNLOAD_START:
			Notification notification1 = new NotificationCompat.Builder(context)
					.setWhen(System.currentTimeMillis())
					.setSmallIcon(R.drawable.icon).setAutoCancel(true)
					.setTicker("开始升级!!").build();
			manager.notify(1, notification1);
			break;
		case DOWNLOAD_RUN:
			Notification notification2 = new NotificationCompat.Builder(context)
					.setContentTitle("升级提示")
					.setWhen(System.currentTimeMillis())
					.setSmallIcon(R.drawable.icon)
					.setContentText("已经下载: " + progress + "%")
					.setAutoCancel(true).build();
			manager.notify(1, notification2);
			break;
		case DOWNLOAD_END:
			Notification notification = new NotificationCompat.Builder(context)
					.setContentTitle("升级提示").setSmallIcon(R.drawable.icon)
					.setWhen(System.currentTimeMillis()).setContentText("下载成功")
					.setAutoCancel(true).build();
			manager.notify(1, notification);
			break;
		}
	}

	public class DownloadCallback implements ITransferCallback {

		private Context context;
		private String url;
		private String path;
		private boolean isForce;
		private DownloadListener mDownloadListener;
		private long startTime;
		
		public DownloadCallback(Context context, String url, String path,
				boolean isForce) {
			super();
			this.context = context;
			this.url = url;
			this.path = path;
			this.isForce = isForce;
		}


		public void setmDownloadListener(DownloadListener mDownloadListener) {
			this.mDownloadListener = mDownloadListener;
		}

		@Override
		public void onTransferReturn(ProtocolType.ResponseEvent responseEvent,
				CommonResult commonResult) {
			System.out.println("====download====path========"+path+"=========commonResult:"+commonResult);
			if (commonResult!= null && !TextUtils.isEmpty(commonResult.urlMd5)
					&& commonResult.urlMd5.equals(BeanUtils.md532(url))) {
				if (responseEvent == ProtocolType.ResponseEvent.SUCCESS
						&& commonResult.progress.completeSize > 0
						&& commonResult.progress.completeSize == commonResult.progress.totalSize) {
					mDownloadListener.onProgress(100);
					if (!isForce) {
						dealDownloadNotification(context, DOWNLOAD_END, 100);
					}
					// 启动升级
					Intent intent = new Intent(Intent.ACTION_VIEW);
					intent.setDataAndType(Uri.fromFile(new File(path)),
							"application/vnd.android.package-archive");
					context.startActivity(intent);
					// 退出当前应用
					// sendExitBroadcast(context);
					MainActivity.this.finish();
					System.exit(0);
				} else if (responseEvent == ProtocolType.ResponseEvent.ERROR) { // download
																				// error.
					// 下载失败等待下次升级重新下载
				} else if (responseEvent == ProtocolType.ResponseEvent.PROGRESS) { // current
																					// download
																					// progress.
					if (commonResult.progress.completeSize > 0
							&& commonResult.progress.completeSize == commonResult.progress.totalSize) {
						if (!isForce) {
							dealDownloadNotification(context, DOWNLOAD_END, 100);
						}
						// 启动升级
						Intent intent = new Intent(Intent.ACTION_VIEW);
						intent.setDataAndType(Uri.fromFile(new File(path)),
								"application/vnd.android.package-archive");
						context.startActivity(intent);
						// 退出当前应用
						// sendExitBroadcast(context);
						mHandler.postDelayed(new Runnable() {
							@Override
							public void run() {
								MainActivity.this.finish();
								System.exit(0);
							}
						}, 200);
						
					} else {
						int progress = (int) (commonResult.progress.completeSize * 100L / commonResult.progress.totalSize);
						mDownloadListener.onProgress(progress);
						if (!isForce) {
							long currentTime = System.currentTimeMillis();
							if (currentTime - startTime > 300) {
								startTime = currentTime;
								dealDownloadNotification(context, DOWNLOAD_RUN,
										progress);
							}
						}
					}
				}
			}
		}
	}

	public interface DownloadListener {
		void onProgress(int progress);
	}
	
	
	private String getAppProcessName(Context context) {
        //当前应用pid
        int pid = android.os.Process.myPid();
        //任务管理类
        ActivityManager manager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        //遍历所有应用
        List<ActivityManager.RunningAppProcessInfo> infos = manager.getRunningAppProcesses();
        for (ActivityManager.RunningAppProcessInfo info : infos) {
            if (info.pid == pid)//得到当前应用
                return info.processName;//返回包名
        }
        return "";
    }
	
	//检测是否安装了应用宝的玩呗斗牌
	//如果安装则不提示
	public void checkApp(){
		//get the package name of this app, if the app's name equals "com.ihuizhi.club.qp"
		//then go the next
		String currentPackName = getAppProcessName(this);
		if ("com.ihuizhi.club.qp".equals(currentPackName)){
			PackageManager manager = getPackageManager();
		    List<PackageInfo> infos = manager.getInstalledPackages(PackageManager.GET_UNINSTALLED_PACKAGES);  
		    boolean isInstall = false;
		    for (PackageInfo packageInfo : infos) {  
		        String packageName = packageInfo.packageName;  
		        if ("com.tencent.tmgp.wbdp".equals(packageName)){
		        	isInstall = true;
		        }
		    }  
		    if (isInstall){
		    	//if the app is installed, then show the uninstall tips;
		    	new AlertDialog.Builder(MainActivity.this)
				.setCancelable(false)
				.setMessage("检测到本机已安装最新版玩呗斗牌\n当前版本已过期，请手动卸载\n不便之处敬请谅解！")
				.setPositiveButton("我知道了",
						new DialogInterface.OnClickListener() {

							@Override
							public void onClick(
									DialogInterface mdialog, int which) {
//								mdialog.dismiss();
								Uri packageURI = Uri.parse("package:com.ihuizhi.club.qp");   
								Intent uninstallIntent = new Intent(Intent.ACTION_DELETE, packageURI);   
								startActivity(uninstallIntent);
								MainActivity.this.finish();
								System.exit(0);
							}
						}).show();
//				.setNegativeButton("退出应用",
//						new DialogInterface.OnClickListener() {
//
//							@Override
//							public void onClick(
//									DialogInterface mdialog, int which) {
//								
//								mdialog.dismiss();
//								MainActivity.this.finish();
//								System.exit(0);
//							}
//						}).show();
		    } 
		}
	}  

    
    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
    	//点击返回，当前应用不在最上层，造成回调拉起退出应用或者退出游戏的对话框
    	if(event.getKeyCode() == KeyEvent.KEYCODE_BACK  && !isOnTop){  
    		return true;
    	}
//    	Log.e("onKeyDown","MainActivity======dispatchKeyEvent========"+(isOnTop? "TRUE":"FALSE"));
    	return super.dispatchKeyEvent(event);
    }
    
    static{
    	try {
            System.loadLibrary("cocos2dcpp");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
}