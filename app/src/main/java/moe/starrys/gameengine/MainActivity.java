package moe.starrys.gameengine;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import moe.starrys.gameengine.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    GL2JNIView mView;

    // Used to load the 'game_engine' library on application startup.
    static {
        System.loadLibrary("game_engine");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView = new GL2JNIView(getApplication());
        setContentView(mView);
        mView.setNativeAssetManager(getAssets());
    }


    /**
     * A native method that is implemented by the 'gameengine' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}