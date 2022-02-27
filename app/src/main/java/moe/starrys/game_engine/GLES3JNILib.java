package moe.starrys.game_engine;

// Wrapper for native library

import android.content.res.AssetManager;

public class GLES3JNILib {

    static {
        System.loadLibrary("game_engine");
    }

    public static native void init();
    public static native void step();
    public static native void setNativeAssetManager(AssetManager assetManager);
    public static native void updateBufferSize(int width, int height);
    public static native int setMobileName(String sName);
    public static native int cameraViewEvent(float xOffset, float yOffset, boolean constrainPitch);
}
