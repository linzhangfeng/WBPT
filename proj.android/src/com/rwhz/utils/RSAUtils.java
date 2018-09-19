package com.rwhz.utils;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.spec.X509EncodedKeySpec;

import javax.crypto.Cipher;

import android.annotation.SuppressLint;
import android.content.Context;
import android.text.TextUtils;

/**
 * RSA
 * @author hexh
 *
 */
@SuppressLint("TrulyRandom")
public class RSAUtils {
    
	private static String mPublicKey = "";
    
    private static PublicKey getPublicKeyFromX509(String algorithm, String bysKey) throws NoSuchAlgorithmException, Exception {
        byte[] decodedKey = Base64Utils.decode(bysKey);
        X509EncodedKeySpec x509 = new X509EncodedKeySpec(decodedKey);
        KeyFactory keyFactory = KeyFactory.getInstance(algorithm);
        return keyFactory.generatePublic(x509);
    }

    public static String encrypt(Context context, String content) {
    	if (TextUtils.isEmpty(mPublicKey)) {
	        init(context);
        }
    	return encrypt(content, mPublicKey);
    }
    
    public static String encrypt(String content, String key) {
        try {
            PublicKey pubkey = getPublicKeyFromX509("RSA", key);
            Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
            cipher.init(Cipher.ENCRYPT_MODE, pubkey);
            byte plaintext[] = content.getBytes("UTF-8");
            byte[] output = cipher.doFinal(plaintext);

            return new String(Base64Utils.encode(output));
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
    
    public static synchronized void init(Context context) {
    	if (TextUtils.isEmpty(mPublicKey)) {
    		InputStream is = null;
    		try {
    			is = context.getResources().getAssets().open("rsa.pem");
    			BufferedReader br = new BufferedReader(new InputStreamReader(is));
    			String readLine = null;
    			StringBuilder sb = new StringBuilder();
    			while ((readLine = br.readLine()) != null) {
    				if (readLine.charAt(0) == '-') {
    					continue;
    				} else {
    					sb.append(readLine);
    					sb.append('\r');
    				}
    			}
    			mPublicKey = sb.toString();
    		} catch (Exception e) {
    	        e.printStackTrace();
            } finally {
            	if (is != null) {
    	            try {
    	                is.close();
                    } catch (Exception e) {
    	                e.printStackTrace();
                    }
                }
            }   
        }    	
	}
}
