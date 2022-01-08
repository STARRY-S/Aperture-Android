#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "main.h"
#include "renderer.h"

static AAssetManager *pLocalAAsetManager = NULL;

JNIEXPORT jstring JNICALL
Java_moe_starrys_gameengine_MainActivity_stringFromJNI(
        JNIEnv* env,jobject /* this */)
{
    const char* hello = "Hello from C";
    return (*env)->NewStringUTF(env, hello);
}

JNIEXPORT void JNICALL
Java_moe_starrys_gameengine_GL2JNILib_init(
        JNIEnv *env, jclass clazz, jint width, jint height) {
    // TODO: implement init()
    setup(width, height);
}

JNIEXPORT void JNICALL
Java_moe_starrys_gameengine_GL2JNILib_step(
        JNIEnv *env, jclass clazz) {
    // TODO: implement step()
    render();
}

JNIEXPORT void JNICALL
Java_moe_starrys_gameengine_GL2JNILib_setNativeAssetManager(
        JNIEnv *env, jclass clazz, jobject asset_manager) {
    // TODO: implement setNativeAssetManager()
    AAssetManager *manager = AAssetManager_fromJava(env, asset_manager);
    setAAssetManager(manager);
}

JNIEXPORT void JNICALL
Java_moe_starrys_gameengine_GL2JNILib_updateBufferSize(JNIEnv *env, jclass clazz, jint width,
                                                       jint height) {
    // TODO: implement updateBufferSize()
    resizeBuffer(width, height);
}