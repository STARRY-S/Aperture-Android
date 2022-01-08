//
// Created by starry-s on 1/8/22.
//

#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#ifndef __ANDROID__
#include <GLFW/glfw3.h>
#endif

#include <stdio.h>
#include <stdbool.h>

GLuint load_texture(const char *const path, int format);

#endif //GAMEENGINE_TEXTURE_H
