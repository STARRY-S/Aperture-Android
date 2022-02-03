//
// Created by starry-s on 1/8/22.
//

#ifndef GAMEENGINE_MAIN_H
#define GAMEENGINE_MAIN_H

#include "cglm/cglm.h"

#ifdef __ANDROID__
#include <android/log.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)

void *getLocalAAssetManager();
int setAAssetManager(void *pManager);

#else   // IFDEF ANDROID

// Output log files to stdout/stderr
#define LOGI(...) fprintf(stdout, __VA_ARGS__);
#define LOGD(...) fprintf(stdout, __VA_ARGS__);
#define LOGE(...) fprintf(stderr, __VA_ARGS__);
#define LOGW(...) fprintf(stdout, __VA_ARGS__);

#endif  // NOT ANDROID

#endif //GAMEENGINE_MAIN_H
