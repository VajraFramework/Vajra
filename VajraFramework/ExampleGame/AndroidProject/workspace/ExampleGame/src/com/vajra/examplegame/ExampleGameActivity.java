/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.vajra.examplegame;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.WindowManager;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import com.vajra.androidwrapper.VajraAndroidWrapper;


public class ExampleGameActivity extends Activity {

    ExampleGameView mView;
    public static Context mContext;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new ExampleGameView(getApplication(), false, 8, 8);
        setContentView(mView);

    	mContext = getApplicationContext();

        VajraAndroidWrapper.SetContext(mContext);
        ExampleGameWrapper.SetContext(mContext);

        int number = VajraAndroidWrapper.TestFunction();
        Log.i("tag", "Number: " + Integer.toString(number));
    }

    @Override
	public boolean onTouchEvent(MotionEvent event) {
    	VajraAndroidWrapper.reportTouchEvent(event);
        return true;
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}
