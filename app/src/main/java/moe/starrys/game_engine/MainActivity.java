package moe.starrys.game_engine;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

public class MainActivity extends AppCompatActivity {
    static String TAG = "ACTIVITY_MAIN";
    private FloatingActionButton btn_move_up = null;
    private FloatingActionButton btn_move_down = null;
    private FloatingActionButton btn_move_left = null;
    private FloatingActionButton btn_move_right = null;
    private FloatingActionButton btn_move_center = null;
    private FloatingActionButton btn_move_run = null;
    private FloatingActionButton btn_move_jump = null;
    private FloatingActionButton btn_light = null;

    private static boolean spotLightEnabled = false;

    // Used to load the 'game_engine' library on application startup.
    static {
        System.loadLibrary("aperture");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        hideSystemBars();
        Toast.makeText(this, "Loading...", Toast.LENGTH_SHORT).show();
//        Snackbar.make(view, "Here's a Snackbar", Snackbar.LENGTH_LONG)
//                .setAction("Action", null).show();
        setContentView(R.layout.activity_main);

        GLES3JNILib.setNativeAssetManager(getAssets());
        GLES3JNILib.setMobileName(getDeviceName());

        btn_move_up = findViewById(R.id.btn_move_up);
        btn_move_up.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GLES3JNILib.setBtnUpPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GLES3JNILib.setBtnUpPressed(false);
                }
                v.performClick();
                return false;
            }
        });
        btn_move_down = findViewById(R.id.btn_move_down);
        btn_move_down.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GLES3JNILib.setBtnDownPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GLES3JNILib.setBtnDownPressed(false);
                }
                v.performClick();
                return false;
            }
        });
        btn_move_left = findViewById(R.id.btn_move_left);
        btn_move_left.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GLES3JNILib.setBtnLeftPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GLES3JNILib.setBtnLeftPressed(false);
                }
                v.performClick();
                return false;
            }
        });
        btn_move_right = findViewById(R.id.btn_move_right);
        btn_move_right.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GLES3JNILib.setBtnRightPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GLES3JNILib.setBtnRightPressed(false);
                }
                v.performClick();
                return false;
            }
        });
        btn_move_center = findViewById(R.id.btn_move_center);
        btn_move_center.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GLES3JNILib.setBtnCenterPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GLES3JNILib.setBtnCenterPressed(false);
                }
                v.performClick();
                return false;
            }
        });
        btn_move_run = findViewById(R.id.btm_move_run);
        btn_move_run.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GLES3JNILib.setBtnRunPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GLES3JNILib.setBtnRunPressed(false);
                }
                v.performClick();
                return false;
            }
        });
        btn_move_jump = findViewById(R.id.btn_move_jump);
        btn_move_jump.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    GLES3JNILib.setBtnJumpPressed(true);
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    GLES3JNILib.setBtnJumpPressed(false);
                }
                v.performClick();
                return false;
            }
        });
        btn_light = findViewById(R.id.btn_light);
        btn_light.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                spotLightEnabled = !spotLightEnabled;
                GLES3JNILib.setSpotLightEnabled(spotLightEnabled);
            }
        });
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