package moe.starrys.gameengine;

// Wrapper for native library

import android.content.res.AssetManager;

public class GL3JNILib {

    static {
        System.loadLibrary("game_engine");
    }

    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);
    public static native void step();
    public static native void setNativeAssetManager(AssetManager assetManager);
    public static native void updateBufferSize(int width, int height);
}
