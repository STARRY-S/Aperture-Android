/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package moe.starrys.game_engine;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLES3JNIView extends GLSurfaceView {
    private static final String TAG = "GLES3View";

    private static boolean firstTouch = true;
    private static float lastX = 0.0f;
    private static float lastY = 0.0f;

    private static Renderer renderer = null;

    public GLES3JNIView(Context context) {
        super(context);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 8, 24, 8);
        setEGLContextClientVersion(3);
        renderer = new Renderer();
        setRenderer(renderer);
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // MotionEvent reports input details from the touch screen
        // and other input controls. In this case, you are only
        // interested in events where the touch position changed.

        if (e.getAction() == MotionEvent.ACTION_MOVE) {
            float xPos = e.getX();
            float yPos = e.getY();

            if(firstTouch)
            {
                lastX = xPos;
                lastY = yPos;
                firstTouch = false;
            }

            float xOffset = xPos - lastX;
            float yOffset = lastY - yPos;
            lastX = xPos;
            lastY = yPos;

            GLES3JNILib.cameraViewEvent(xOffset, yOffset, true);
        } else if (e.getAction() == MotionEvent.ACTION_UP) {
            firstTouch = true;
        }

        return true;
    }

    private static class Renderer implements GLSurfaceView.Renderer {
        private static final String TAG = "GLES3Render";

        @Override
        public void onDrawFrame(GL10 gl) {
            GLES3JNILib.step();
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            GLES3JNILib.updateBufferSize(width, height);
        }

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
             GLES3JNILib.init();
        }
    }
}

