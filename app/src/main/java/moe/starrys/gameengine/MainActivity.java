package moe.starrys.gameengine;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import moe.starrys.gameengine.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    GLES3JNIView mView;

    // Used to load the 'game_engine' library on application startup.
    static {
        System.loadLibrary("game_engine");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView = new GLES3JNIView(getApplication());
        setContentView(mView);
        GLES3JNILib.setNativeAssetManager(getAssets());
    }
    private ActivityMainBinding binding;
}