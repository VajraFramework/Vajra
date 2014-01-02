package com.vajra.androidwrapper;

import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

// Wrapper for the Vajra native library
public class VajraAndroidWrapper {

	public static void SetContext(Context context) {
		VajraAndroidWrapper.savedContext = context;
	}
	
	public static Context GetContext() {
		return VajraAndroidWrapper.savedContext;
	}
	
	static {
		System.loadLibrary("Vajra");
	}

	public static void TestMethodInJava() {
		Log.d("tag", "CAME HERE TO JAVA");
		return;
	}

	public static byte[] getAssetContents(String path)
			throws IOException {

		InputStream is = VajraAndroidWrapper.GetContext().getAssets().open(path, AssetManager.ACCESS_BUFFER);
		byte buffer[] = new byte[is.available()];
		is.read(buffer);
		return buffer;
	}


	public static int TestFunction() {
		return 4;
	}
	
	public static String GetDeviceResourcesBasePath() {
		return AssetCopier.GetDeviceResourcesBasePath(VajraAndroidWrapper.savedContext);
	}

	public static void CopyAssetsToSDCard() {
		AssetCopier.CopyAssetsToSDCard(VajraAndroidWrapper.savedContext);
	}

	private static Context savedContext;

	public static native void init(int width, int height);
	public static native void step(float dt);
}
