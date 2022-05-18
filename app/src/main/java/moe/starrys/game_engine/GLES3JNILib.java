package moe.starrys.game_engine;

// Wrapper for native library

import android.content.res.AssetManager;

public class GLES3JNILib {

    public static final int MOVE_FORWARD = 0;
    public static final int MOVE_BACKWARD = 1;
    public static final int MOVE_LEFT = 2;
    public static final int MOVE_RIGHT = 3;

    static {
        System.loadLibrary("aperture");
    }

    private static boolean btnDownPressed;
    private static boolean btnUpPressed;
    private static boolean btnLeftPressed;
    private static boolean btnRightPressed;
    private static boolean btnCenterPressed;
    private static boolean btnJumpPressed;
    private static boolean btnRunPressed;
    private static boolean spotLightEnabled;

    public static native void init();
    public static native void step();
    public static native void setNativeAssetManager(AssetManager assetManager);
    public static native void updateBufferSize(int width, int height);
    public static native int setMobileName(String sName);
    public static native int cameraViewEvent(float xOffset, float yOffset, boolean constrainPitch);
    public static native int creatureMove(int direction, float speed);
    public static native int creatureJump();
    public static native int spotLightStatus(boolean status);

    public static void processEvents() {
        float speed = btnRunPressed ? 2.0f : 1.0f;
        if (btnUpPressed) {
            creatureMove(MOVE_FORWARD, speed);
        }
        if (btnDownPressed) {
            creatureMove(MOVE_BACKWARD, speed);
        }
        if (btnLeftPressed) {
            creatureMove(MOVE_LEFT, speed);
        }
        if (btnRightPressed) {
            creatureMove(MOVE_RIGHT, speed);
        }
        if (btnCenterPressed || btnJumpPressed) {
            creatureJump();
        }
        spotLightStatus(spotLightEnabled);
    }

    public static void setBtnDownPressed(boolean btnDownPressed) {
        GLES3JNILib.btnDownPressed = btnDownPressed;
    }

    public static void setBtnUpPressed(boolean btnUpPressed) {
        GLES3JNILib.btnUpPressed = btnUpPressed;
    }

    public static void setBtnLeftPressed(boolean btnLeftPressed) {
        GLES3JNILib.btnLeftPressed = btnLeftPressed;
    }

    public static void setBtnRightPressed(boolean btnRightPressed) {
        GLES3JNILib.btnRightPressed = btnRightPressed;
    }

    public static void setBtnCenterPressed(boolean btnCenterPressed) {
        GLES3JNILib.btnCenterPressed = btnCenterPressed;
    }

    public static void setBtnJumpPressed(boolean btnJumpPressed) {
        GLES3JNILib.btnJumpPressed = btnJumpPressed;
    }

    public static void setBtnRunPressed(boolean btnRunPressed) {
        GLES3JNILib.btnRunPressed = btnRunPressed;
    }

    public static void setSpotLightEnabled(boolean spotLightEnabled) {
        GLES3JNILib.spotLightEnabled = spotLightEnabled;
    }
}
