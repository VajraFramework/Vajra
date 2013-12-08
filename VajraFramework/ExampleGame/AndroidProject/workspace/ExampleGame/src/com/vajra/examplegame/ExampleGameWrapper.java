package com.vajra.examplegame;

import android.content.Context;

public class ExampleGameWrapper {

	public static void SetContext(Context context) {
		ExampleGameWrapper.savedContext = context;
	}
	
	public static Context GetContext() {
		return ExampleGameWrapper.savedContext;
	}
	
	static {
		System.loadLibrary("ExampleGame");
	}
	
	private static Context savedContext;

	public static native void InitGameScene();
}
