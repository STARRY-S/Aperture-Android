package moe.starrys.game_engine;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import android.os.Bundle;
import moe.starrys.game_engine.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    GLES3JNIView mView;

    // Used to load the 'game_engine' library on application startup.
    static {
        System.loadLibrary("game_engine");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        hideSystemBars();
        mView = new GLES3JNIView(getApplication());
        setContentView(mView);
        GLES3JNILib.setNativeAssetManager(getAssets());
    }
    private ActivityMainBinding binding;

    private void hideSystemBars() {
        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Configure the behavior of the hidden system bars
        windowInsetsController.setSystemBarsBehavior(
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        );
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
    }
}