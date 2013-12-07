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

import java.io.IOException;
import java.io.InputStream;

import android.content.res.AssetManager;
import android.util.Log;


// Wrapper for native library

public class ExampleGameLib {

     static {
         // System.loadLibrary("gl2jni");
         System.loadLibrary("Vajra");
     }

     public static void TestMethodInJava() {
    	 Log.d("tag", "CAME HERE TO JAVA");
    	 
    	 return;
     }

     public static byte[] getAssetContents(String path) throws IOException
     {
    	 InputStream is = ExampleGameActivity.mContext.getAssets().open(path, AssetManager.ACCESS_BUFFER);
    	 byte buffer[] = new byte[is.available()];
    	 is.read(buffer);
    	 return buffer;
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height);
     public static native void step(float dt);
}
