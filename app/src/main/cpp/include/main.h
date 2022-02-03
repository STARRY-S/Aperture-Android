//
// Created by starry-s on 1/8/22.
//

#ifndef GAMEENGINE_MAIN_H
#define GAMEENGINE_MAIN_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cglm/cglm.h"

#ifdef __ANDROID__
#include <android/log.h>

#define  LOG_TAG    "GLES3JNI"
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

#endif  // __ANDROID__

// common used error types
typedef enum {
    GE_ERROR_SUCCESS = 0,
    GE_ERROR_INVALID_POINTER,   // pointer is NULL
    GE_ERROR_INVALID_PARAMETER, // invalid param
    GE_ERROR_MALLOC_FAILED,     // malloc failed
    GE_ERROR_MESH_UNINITIALIZED,// mesh uninitialized
    GE_ERROR_INIT_FAILED,       // initialize failed
    GE_ERROR_RENDER_FAILED,
    GE_ERROR_TEXTURE_FAILED,
    GE_ERROR_UNKNOWN,
    GE_ERROR_LENGTH                 // never use this
} GE_Types;

#endif //GAMEENGINE_MAIN_H
