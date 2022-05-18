package moe.starrys.game_engine;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

import android.annotation.SuppressLint;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.text.Layout;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

public class MainActivity extends AppCompatActivity {
    static String TAG = "ACTIVITY_MAIN";

    private static int textureNum = 0;
    private static boolean spotLightEnabled = false;

    // Used to load the 'game_engine' library on application startup.
    static {
        System.loadLibrary("aperture");
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        Toast.makeText(this, "Loading...", Toast.LENGTH_SHORT).show();
        setContentView(R.layout.activity_main);

        GLES3JNILib.setNativeAssetManager(getAssets());
        GLES3JNILib.setMobileName(getDeviceName());

        FloatingActionButton btn_move_up = findViewById(R.id.btn_move_up);
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
        FloatingActionButton btn_move_down = findViewById(R.id.btn_move_down);
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
        FloatingActionButton btn_move_left = findViewById(R.id.btn_move_left);
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
        FloatingActionButton btn_move_right = findViewById(R.id.btn_move_right);
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
        FloatingActionButton btn_move_center = findViewById(R.id.btn_move_center);
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
        FloatingActionButton btn_move_run = findViewById(R.id.btm_move_run);
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
        FloatingActionButton btn_move_jump = findViewById(R.id.btn_move_jump);
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
        FloatingActionButton btn_light = findViewById(R.id.btn_light);
        btn_light.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                spotLightEnabled = !spotLightEnabled;
                String msg = null;
                if (spotLightEnabled) {
                    msg = "Turn on the spot light.";
                } else {
                    msg = "Turn off the spot light.";
                }
                Snackbar.make(v, msg, Snackbar.LENGTH_SHORT)
                        .setAction("Action", null).show();
                GLES3JNILib.setSpotLightEnabled(spotLightEnabled);
            }
        });
        FloatingActionButton btn_texture_change = findViewById(R.id.btn_texture_change);
        btn_texture_change.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // only have two textures currently
                textureNum = (textureNum > 0) ? 0 : 1;
                Snackbar.make(v, "Set material number: " + textureNum, Snackbar.LENGTH_SHORT)
                    .setAction("Action", null).show();
                GLES3JNILib.changeTextureNum(textureNum);
            }
        });
    }

    // Refer https://developer.android.com/training/system-ui/immersive#EnableFullscreen
    private void hideSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN);
    }

    // Shows the system bars by removing all the flags
    // except for the ones that make the content appear under the system bars.
    private void showSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        } else {
            showSystemUI();
        }
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