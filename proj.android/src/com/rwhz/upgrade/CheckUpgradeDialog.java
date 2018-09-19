package com.rwhz.upgrade;

import com.ihuizhi.club.qp.R;
import com.rwhz.app.MainActivity;
import com.rwhz.app.MainActivity.DownloadListener;
import com.rwhz.utils.Utils;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.os.Handler;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;


/**
 * Created by Niklaus on 2016/7/15.
 * 升级Dialog
 */
public class CheckUpgradeDialog extends Dialog implements DownloadListener{

	private Context context;
	private View view;
    private TextView tvVersion;
    private TextView tvSize;
    private TextView tvDesc;
    private NumberProgressBar progressBar;
    private TextView negtiveText;
    public boolean force;
    public String name;
    public String size;
    public String desc;
    
    private OnUpgradeListener mOnUpgradeListener;
    
    public void setmOnUpgradeListener(OnUpgradeListener mOnUpgradeListener) {
		this.mOnUpgradeListener = mOnUpgradeListener;
	}
    
    public CheckUpgradeDialog(Context context, int theme) {
		super(context, theme);
		this.context = context;
		initUI();
	}

	public CheckUpgradeDialog(Context context) {
		this(context, R.style.DialogTheme);
		this.context = context;
		initUI();
	}
	
	private void initUI(){
		setContentView(R.layout.dialog_upgrade);
		WindowManager.LayoutParams params = getWindow().getAttributes();
        params.width = Utils.dip2px(getContext(), 300f);
        params.gravity = Gravity.CENTER;
        view = getWindow().getDecorView();
		
		tvVersion = (TextView) view.findViewById(R.id.upgrade_version);
        tvSize = (TextView) view.findViewById(R.id.upgrade_size);
        tvDesc = (TextView) view.findViewById(R.id.upgrade_desc);
        progressBar = (NumberProgressBar) view.findViewById(R.id.download_progress);
       
        negtiveText = (TextView) view.findViewById(R.id.negative);
        findViewById(R.id.positive).setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				view.setEnabled(false);
				if (mOnUpgradeListener != null){
					mOnUpgradeListener.onPositiveClick();
				}
				
			}
		});
        negtiveText.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				view.setEnabled(false);
				dismiss();
				if (mOnUpgradeListener != null){
					mOnUpgradeListener.onNegtiveClick();
				}
			}
		});
	}
	
	public void refreshUI(){
		initUI();
		setForce(force);
		setVersion(name);
		setSize(size);
		setDesc(desc);
	}
	
	public void setForce(boolean force) {
		this.force = force;
		 if (force){
	        negtiveText.setText("退出应用");
	        this.setCancelable(false);
	     }else{
	    	 this.setCancelable(true);
	     }
	}
	
	public void setVersion(String name){
		this.name = name;
		if (!TextUtils.isEmpty(name)){
			tvVersion.setText(name);
		}
	}
	
	public void setSize(String size){
		this.size = size;
		if (!TextUtils.isEmpty(size)){
			tvSize.setText(size);
		}
	}
	
	public void setDesc(String desc){
		this.desc = desc;
		if (!TextUtils.isEmpty(desc)){
			tvDesc.setText(desc);
		}
	}
	
	public interface OnUpgradeListener{
		void onPositiveClick();
		void onNegtiveClick();
		
	}

	@Override
	public void onProgress(final int progress) {
		if (progressBar != null){
			System.err.println("progress==="+progress);
			Activity activity = (Activity) context;
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					progressBar.setProgress(progress);
				}
			});
		}
	}
	
	public CheckUpgradeDialog clone(){
		CheckUpgradeDialog dialog = new CheckUpgradeDialog(this.context);
		dialog.setDesc(this.desc);
		dialog.setForce(this.force);
		dialog.setSize(this.size);
		dialog.setVersion(this.name);
		dialog.setmOnUpgradeListener(this.mOnUpgradeListener);
		return dialog;
	}
	
}
