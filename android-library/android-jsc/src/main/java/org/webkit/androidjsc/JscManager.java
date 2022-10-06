package org.webkit.androidjsc;

import android.content.res.AssetManager;

public class JscManager {
    static {
        System.loadLibrary("jsc");
    }

    public static native void init(AssetManager assetManager);
}
