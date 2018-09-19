package com.rwhz.utils;


import java.util.HashMap;

import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.text.TextUtils;
import android.util.Log;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.Platform.ShareParams;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.sina.weibo.SinaWeibo;
import cn.sharesdk.tencent.qq.QQ;
import cn.sharesdk.tencent.qzone.QZone;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.wechat.moments.WechatMoments;

import com.ihuizhi.club.qp.R;

public class ShareMgr {

    // http://www.xyz.html?su=1&ss=clubqp&sf=0&st=10

    /**
     * 分享者
     */
    static final String KEY_SHARE_USER = "su";
    /**
     * 分享源
     */
    static final String KEY_SHARE_SOURCE = "ss";
    /**
     * 分享自
     */
    static final String KEY_SHARE_FROM = "sf";
    /**
     * 分享到
     */
    static final String KEY_SHARE_TO = "st";

    static final String VALUE_SHARE_SOURCE = "clubqp";
    static final ShareFrom VALUE_SHARE_FROM = ShareFrom.Android;

    static Context mContext = null;

    public static void init(Context context) {
        mContext = context;
        ShareSDK.initSDK(context);
    }

    public static void share(ShareTo to, String title, String text, String image, String url) {
        switch (to) {
//            case SMS:
//                shareToSMS(title, text, image, makeUrl(to, url));
//                break;
//
//            case WX:
//                shareToWX(title, text, image, makeUrl(to, url));
//                break;

//            case WXZONE:
//                shareToWXZone(title, text, image, makeUrl(to, url));
//                break;
//
//            case QQ:
//                shareToQQ(title, makeUrl(to, url), text, image);
//                break;
//
//            case QQZONE:
//                shareToQQZone(title, makeUrl(to, url), text, image, null, null);
//                break;
//
//            case SINAWEIBO:
//                shareToSinaWeibo(title, makeUrl(to, url));
//                break;
//
//            case QQWEIBO:
//                shareToQQWeibo(title, makeUrl(to, url));
//                break;

            default:
                break;
        }
    }

//    public static void recommendApp(ShareTo to) {
//        switch (to) {
//            case SMS:
//                shareToSMS(getRecommendAppTitle(),
//                        getRecommendAppContent(),
//                        getRecommendAppImage(),
//                        makeUrl(to, getRecommendAppUrl()));
//                break;
//
//            case WX:
//                shareToWX(getRecommendAppTitle(),
//                        getRecommendAppContent(),
//                        getRecommendAppImage(),
//                        makeUrl(to, getRecommendAppUrl()));
//                break;
//
//            case WXZONE:
//                shareToWXZone(getRecommendAppTitle(),
//                        getRecommendAppContent(),
//                        getRecommendAppImage(),
//                        makeUrl(to, getRecommendAppUrl()));
//                break;
//
//            case QQ:
//                shareToQQ(getRecommendAppTitle(),
//                        makeUrl(to, getRecommendAppUrl()),
//                        getRecommendAppContent(),
//                        getRecommendAppImage());
//                break;
//
//            case QQZONE:
//                shareToQQZone(getRecommendAppTitle(),
//                        makeUrl(to, getRecommendAppUrl()),
//                        getRecommendAppContent(),
//                        getRecommendAppImage(), null, null);
//                break;
//
//            case SINAWEIBO:
//                shareToSinaWeibo(getRecommendAppTitle() +
//                                getRecommendAppContent(),
//                        makeUrl(to, getRecommendAppUrl()));
//                break;
//
//            case QQWEIBO:
//                shareToQQWeibo(getRecommendAppTitle() +
//                                getRecommendAppContent(),
//                        makeUrl(to, getRecommendAppUrl()));
//                break;
//
//            default:
//                break;
//        }
//    }
//
//    public static void recommendFriend(ShareTo to) {
//        switch (to) {
//            case SMS:
//                shareToSMS(getRecommendFriendTitle(), null,
//                        getRecommendFriendImage(),
//                        makeUrl(to, getRecommendFriendUrl()));
//                break;
//
//            case WX:
//                shareToWX(getRecommendFriendTitle(), null,
//                        getRecommendFriendImage(),
//                        makeUrl(to, getRecommendFriendUrl()));
//                break;
//
//            case WXZONE:
//                shareToWXZone(getRecommendFriendTitle(), null,
//                        getRecommendFriendImage(),
//                        makeUrl(to, getRecommendFriendUrl()));
//                break;
//
//            case QQ:
//                shareToQQ(getRecommendFriendTitle(),
//                        makeUrl(to, getRecommendFriendUrl()), null,
//                        getRecommendFriendImage());
//                break;
//
//            case QQZONE:
//                shareToQQZone(getRecommendFriendTitle(),
//                        makeUrl(to, getRecommendFriendUrl()), null,
//                        getRecommendFriendImage(), null, null);
//                break;
//
//            case SINAWEIBO:
//                shareToSinaWeibo(getRecommendFriendTitle(),
//                        makeUrl(to, getRecommendFriendUrl()));
//                break;
//
//            case QQWEIBO:
//                shareToQQWeibo(getRecommendFriendTitle(),
//                        makeUrl(to, getRecommendFriendUrl()));
//                break;
//
//            default:
//                break;
//        }
//    }

//    public static void recommendInvitation(ShareTo to, String inviteUrl) {
//        switch (to) {
//            case SMS:
//                shareToSMS(getRecommendInvitationTitle(), getRecommendInvitationText(),
//                        getRecommendFriendImage(),
//                        makeUrl(to, inviteUrl));
//                break;
//
//            case WX:
//                shareToWX(getRecommendInvitationTitle(), getRecommendInvitationText(),
//                        getRecommendFriendImage(),
//                        makeUrl(to, inviteUrl));
//                break;
//
//            case WXZONE:
//                shareToWXZone(getRecommendInvitationTitle(), getRecommendInvitationText(),
//                        getRecommendFriendImage(),
//                        makeUrl(to, inviteUrl));
//                break;
//
//            case QQ:
//                shareToQQ(getRecommendInvitationTitle(),
//                        makeUrl(to, inviteUrl), getRecommendInvitationText(),
//                        getRecommendFriendImage());
//                break;
//
//            case QQZONE:
//                shareToQQZone(getRecommendInvitationTitle(),
//                        makeUrl(to, inviteUrl), getRecommendInvitationText(),
//                        getRecommendFriendImage(), null, null);
//                break;
//
//            case SINAWEIBO:
//                shareToSinaWeibo(getRecommendInvitationTitle(),
//                        makeUrl(to, inviteUrl));
//                break;
//
//            case QQWEIBO:
//                shareToQQWeibo(getRecommendInvitationTitle(),
//                        makeUrl(to, inviteUrl));
//                break;
//
//            default:
//                break;
//        }
//    }

    public static void againstHistory(ShareTo to, String imagePath) {
        switch (to) {
            case WX: {
                ShareParams sp = new ShareParams();
                sp.setShareType(Platform.SHARE_IMAGE);
                sp.setTitle("test");
                sp.setImagePath(imagePath);

                Platform platform = ShareSDK.getPlatform(Wechat.NAME);
                platform.setPlatformActionListener(new SimplePlatformActionListener());
                platform.share(sp);
            }
            break;
            case QQ: {
                ShareParams sp = new ShareParams();
                sp.setShareType(Platform.SHARE_IMAGE);
                sp.setImagePath(imagePath);

                Platform platform = ShareSDK.getPlatform(QQ.NAME);
                platform.setPlatformActionListener(new SimplePlatformActionListener());
                platform.share(sp);
            }
            break;
            default:
                break;
        }
    }

    public static void shareToSinaWeibo(String text, String url) {
        ShareParams sp = new ShareParams();
        sp.setText(text + "  " + url);

        Platform platform = ShareSDK.getPlatform(SinaWeibo.NAME);
        platform.setPlatformActionListener(new SimplePlatformActionListener());
        platform.share(sp);
    }

    public static void shareToQQWeibo(String text, String url) {
        /*
        ShareParams sp = new ShareParams();
		sp.setText(text + "  " + url);

		Platform platform = ShareSDK.getPlatform(TencentWeibo.NAME);
		platform.setPlatformActionListener(new SimplePlatformActionListener());
		platform.share(sp);
		*/
    }

    public static void shareToQQ(String title, String titleUrl, String text, String imageUrl) {
        ShareParams sp = new ShareParams();
        sp.setTitle(title);
        sp.setTitleUrl(titleUrl);
        sp.setText(text);
        sp.setImageUrl(imageUrl);

        Platform platform = ShareSDK.getPlatform(QQ.NAME);
        platform.setPlatformActionListener(new SimplePlatformActionListener());
        platform.share(sp);
    }

    public static void shareToQQZone(String title, String titleUrl, String text, String imageUrl, String site, String siteUrl) {
        ShareParams sp = new ShareParams();
        sp.setTitle(title);
        sp.setTitleUrl(titleUrl);
        sp.setText(text);
        sp.setImageUrl(imageUrl);
        sp.setSite(site);
        sp.setSiteUrl(siteUrl);

        Platform platform = ShareSDK.getPlatform(QZone.NAME);
        platform.setPlatformActionListener(new SimplePlatformActionListener());
        platform.share(sp);
    }

    public static void shareToSMS(String title, String text, String imageUrl, String url) {
        Uri smsToUri = Uri.parse("smsto:");
        Intent intent = new Intent(Intent.ACTION_SENDTO, smsToUri);
        String body = "";
        if (!TextUtils.isEmpty(title)) {
            body += title;
        }
        if (!TextUtils.isEmpty(text)) {
            body += text;
        }
        if (!TextUtils.isEmpty(url)) {
            body += url;
        }
        intent.putExtra("sms_body", body);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        mContext.startActivity(intent);
    }

    public static void shareToWX(String title, String text, String imageUrl, String url) {
    	Log.e("shareToWX", title + "////" + text + "////" + imageUrl + "////" + url + "////");
        ShareParams sp = new ShareParams();
        if(TextUtils.isEmpty(url)){
        	sp.setShareType(Platform.SHARE_TEXT);
        }else{
        	sp.setShareType(Platform.SHARE_WEBPAGE);
        }
        sp.setTitle(title);
        sp.setText(text);
//    	if (imageUrl.equals(""))
    	{
//    		imageUrl = mContext.getString(R.drawable.icon);
//    		sp.setImagePath(imageUrl);
    		sp.setImageData(BitmapFactory.decodeResource(mContext.getResources(), R.drawable.icon));
    	}
//    	else
//    	{
//    		if (imageUrl.indexOf("http") != -1)
//    		{
//    			sp.setImageUrl(imageUrl);
//    		}
//    		else
//    		{
//    			sp.setImagePath(imageUrl);
//    		}
//    	}
        
        sp.setUrl(url);

        Platform platform = ShareSDK.getPlatform(Wechat.NAME);
        platform.setPlatformActionListener(new SimplePlatformActionListener());
        platform.share(sp);
    }
    
    public static void shareToWXImage(String title, String imagePath) {
        ShareParams sp = new ShareParams();
        sp.setShareType(Platform.SHARE_IMAGE);
        sp.setTitle(title);
        sp.setImagePath(imagePath);

        Platform platform = ShareSDK.getPlatform(Wechat.NAME);
        platform.setPlatformActionListener(new SimplePlatformActionListener());
        platform.share(sp);
    }
    
    public static void shareToWXFriendsCircleImage(String title, String imagePath) {
        ShareParams sp = new ShareParams();
        sp.setShareType(Platform.SHARE_IMAGE);
        sp.setTitle(title);
        sp.setImagePath(imagePath);

        Platform platform = ShareSDK.getPlatform(WechatMoments.NAME);
        platform.setPlatformActionListener(new SimplePlatformActionListener());
        platform.share(sp);
    }

    public static void shareToWXZone(String title, String text, String imageUrl, String url) {
        ShareParams sp = new ShareParams();
        sp.setShareType(Platform.SHARE_WEBPAGE);
        sp.setTitle(title);
        sp.setText(text);
        sp.setImageUrl(imageUrl);
        sp.setUrl(url);

        Platform platform = ShareSDK.getPlatform(WechatMoments.NAME);
        platform.setPlatformActionListener(new SimplePlatformActionListener());
        platform.share(sp);
    }
//
//    private static String makeUrl(ShareTo to, String url) {
//        if (TextUtils.isEmpty(url)) {
//            return null;
//        }
//
//        StringBuilder sb = new StringBuilder();
//        sb.append(url);
//        if (!url.contains("?")) {
//            sb.append("?");
//        } else {
//            sb.append("&");
//        }
//
//        sb.append(KEY_SHARE_USER).append("=").append(MConfig.getUserId()).append("&");
//        sb.append(KEY_SHARE_SOURCE).append("=").append(VALUE_SHARE_SOURCE).append("&");
//        sb.append(KEY_SHARE_FROM).append("=").append(VALUE_SHARE_FROM.value).append("&");
//        sb.append(KEY_SHARE_TO).append("=").append(to.value);
//
//        return sb.toString();
//    }
//
//    public static String getRecommendAppContent() {
//        String content = MConfig.getRecommendAppContent();
//        if (TextUtils.isEmpty(content)) {
//            content = mContext.getString(R.string.recommend_app_content_default);
//        }
//        return content;
//    }
//
//    public static String getRecommendAppTitle() {
//        String title = MConfig.getRecommendAppTitle();
//        if (TextUtils.isEmpty(title)) {
//            title = mContext.getString(R.string.recommend_app_title_default);
//        }
//        return title;
//    }
//
//    public static String getRecommendAppImage() {
//        String image = MConfig.getRecommendAppImage();
//        if(!TextUtils.isEmpty(image) && !image.startsWith("file://")){
//            image = MConfig.getImageServerAddr() + image;
//        }
//        return image;
//    }
//
//    public static String getRecommendAppUrl() {
//        String url = MConfig.getRecommendAppUrl();
//        if (TextUtils.isEmpty(url)) {
//            url = mContext.getString(R.string.recommend_app_url_default);
//        }
//        return url;
//    }
//
//    public static String getRecommendFriendTitle() {
//        String title = MConfig.getRecommendFriendTitle();
//        if (TextUtils.isEmpty(title)) {
//            title = mContext.getString(R.string.recommend_friend_title_default);
//        }
//        return title;
//    }
//
//    public static String getRecommendFriendImage() {
//        String image = MConfig.getRecommendFriendImage();
//        if(!TextUtils.isEmpty(image) && !image.startsWith("file://")){
//            image = MConfig.getImageServerAddr() + image;
//        }
//        return image;
//    }
//
//    public static String getRecommendFriendUrl() {
//        String url = MConfig.getRecommendFriendUrl();
//        if (TextUtils.isEmpty(url)) {
//            url = mContext.getString(R.string.recommend_friend_url_default);
//        }
//        return url;
//    }
//
//    public static String getRecommendInvitationTitle() {
//        String title = MConfig.getRecommendInvitationTitle();
//        if (TextUtils.isEmpty(title)) {
//            title = mContext.getString(R.string.share_wx_title);
//        }
//        return title;
//    }
//
//    public static String getRecommendInvitationText() {
//        String text = MConfig.getRecommendInvitationText();
//        if (TextUtils.isEmpty(text)) {
//            text = mContext.getString(R.string.share_wx_body);
//        }
//        return text;
//    }

    public static enum ShareFrom {
        Android(0),
        IOS(1),
        Wap(2),
        Web(3);

        int value;

        ShareFrom(int value) {
            this.value = value;
        }

        public int value() {
            return this.value;
        }
    }

    public static enum ShareTo {
        SMS(0),
        WX(10),
        WXZONE(11),
        QQ(20),
        QQZONE(21),
        SINAWEIBO(30),
        QQWEIBO(31);

        int value;

        ShareTo(int value) {
            this.value = value;
        }

        public int value() {
            return this.value;
        }
    }

    public static class SimplePlatformActionListener implements PlatformActionListener {

        @Override
        public void onError(Platform plat, int action, Throwable t) {

        }

        @Override
        public void onComplete(Platform plat, int action, HashMap<String, Object> res) {

        }

        @Override
        public void onCancel(Platform plat, int action) {

        }
    }
}
