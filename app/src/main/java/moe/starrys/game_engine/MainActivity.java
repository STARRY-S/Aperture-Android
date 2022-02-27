package moe.starrys.game_engine;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    static String TAG = "ACTIVITY_MAIN";
    GLES3JNIView mView = null;

    // Used to load the 'game_engine' library on application startup.
    static {
        System.loadLibrary("game_engine");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // hideSystemBars();
        Toast.makeText(this, "Loading...", Toast.LENGTH_LONG).show();
        mView = new GLES3JNIView(getApplication());
        setContentView(mView);

        GLES3JNILib.setNativeAssetManager(getAssets());
        GLES3JNILib.setMobileName(getDeviceName());
    }

    private void hideSystemBars() {
        WindowInsetsControllerCompat windowInsetsController =
                ViewCompat.getWindowInsetsController(getWindow().getDecorView());
        if (windowInsetsController == null) {
            return;
        }
        // Configure the behavior of the hidden system bars
        windowInsetsController.setSystemBarsBehavior(
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_BARS_BY_SWIPE
        );
        // Hide both the status bar and the navigation bar
        windowInsetsController.hide(WindowInsetsCompat.Type.systemBars());
    }

    public String getDeviceName() {
        String manufacturer = Build.MANUFACTURER;
        String model = Build.MODEL;
        if (model.toLowerCase().startsWith(manufacturer.toLowerCase())) {
            return model.toLowerCase();
        } else {
            return manufacturer.toLowerCase() + " " + model.toLowerCase();
        }
    }
}