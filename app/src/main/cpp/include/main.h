//
// Created by starry-s on 1/8/22.
//

#ifndef GAME_ENGINE_MAIN_H
#define GAME_ENGINE_MAIN_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cglm/cglm.h"

#ifdef __ANDROID__
#include <android/log.h>

#define  LOG_TAG    "GE_MAIN"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)

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
    GE_ERROR_ASSIMP_IMPORT_FAILED, // assimp import failed
    GE_ERROR_ASSET_OPEN_FAILED,     // asset manager open file failed
    GE_ERROR_INIT_FAILED,       // initialize failed
    GE_ERROR_RENDER_FAILED,
    GE_ERROR_TEXTURE_FAILED,
    GE_ERROR_UNKNOWN,
    GE_ERROR_LENGTH                 // never use this
} GE_Types;

static const char *GE_ERROR_NAME[] = {
        "SUCCESS",
        "INVALID_POINTER",
        "INVALID_PARAMETER",
        "MALLOC_FAILED",
        "MESH_UNINITIALIZED",
        "ASSET_OPEN_FAILED",
        "ASSIMP_IMPORT_FAILED",
        "INIT_FAILED",
        "RENDER_FAILED",
        "TEXTURE_FAILED",
        "UNKNOWN"
};

#define GE_CHECK(code) if (code > 1 && code < GE_ERROR_LENGTH) LOGE("%s", GE_ERROR_NAME[code])

// global error No.
extern int GE_errorno;

void *getLocalAAssetManager();
int setAAssetManager(void *pManager);

#endif //GAME_ENGINE_MAIN_H
