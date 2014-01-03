package com.vajra.androidwrapper;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

public class AssetCopier {

	public static String GetDeviceResourcesBasePath(Context context) {
		String deviceResourcesPath = context.getExternalFilesDir(null).getAbsolutePath() + "/Resources/";
		return deviceResourcesPath;
	}

	public static void CopyAssetsToSDCard(Context context) {
		// TODO [Implement] Automate the generation of this list
		String basePath = "Resources/";
		String[] paths = {
				"animations",
				"armatures",
				"audio",
				"fonts",
				"logging",
				"models",
				"pictures",
				"shaders",
				"uiscenes",
		};
		for (String path : paths) {
			Log.i("Vajra", "About to copy " + path + " assets to sdcard");
			AssetCopier.copyAssetFiles(basePath, path, context);
		}
	}

    private static void copyAssetFiles(String basePath, String subPath, Context context) {
    	File baseDirectory = new File(GetDeviceResourcesBasePath(context), subPath);
    	baseDirectory.mkdirs();

    	AssetManager assetManager = context.getAssets();
        String[] files = null;
        try {
            files = assetManager.list(basePath + subPath);
        } catch (IOException e) {
            Log.e("Vajra", "Failed to get asset file list.", e);
        }

        for(String filename : files) {
            InputStream in = null;
            OutputStream out = null;
            try {
              in = assetManager.open(basePath + subPath + "/" + filename);
              File outFile = new File(GetDeviceResourcesBasePath(context) + subPath + "/", filename);
              out = new FileOutputStream(outFile);
              copyFile(in, out);
              in.close();
              in = null;
              out.flush();
              out.close();
              out = null;
            } catch(IOException e) {
                Log.e("Vajra", "Failed to copy asset file: " + filename, e);
            }       
        }
    }

    private static void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
          out.write(buffer, 0, read);
        }
    }

}
