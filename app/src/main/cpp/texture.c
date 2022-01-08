#include "texture.h"
#include "main.h"

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint load_texture(const char *const path, int format)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = NULL;
#ifdef __ANDROID__
    AAssetManager *pManager = getLocalAAssetManager();
    AAsset *pathAsset = AAssetManager_open(pManager, path, AASSET_MODE_UNKNOWN);
    off_t assetLength = AAsset_getLength(pathAsset);
    unsigned char *fileData = (unsigned char *) AAsset_getBuffer(pathAsset);
    data = stbi_load_from_memory(fileData, assetLength, &width, &height, &nrChannels, 0);
    AAsset_close(pathAsset);
    LOGE("path %s width: %d, height: %d, channel %d\n", path, width, height, nrChannels);
#else
    data = stbi_load(path, &width, &height, &nrChannels, 0);
#endif
    if (data) {
        // target, level, format,
        // w, h, 0(always), origin_format,
        // data_format, data
        glTexImage2D(GL_TEXTURE_2D, 0, format,
                     width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        LOGI("Loaded texture: %s\n", path);
    } else {
        LOGE("Failed to load texture: %s\n", path);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture;
}
