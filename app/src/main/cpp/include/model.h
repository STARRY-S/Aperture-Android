#ifndef __MODEL_H__
#define __MODEL_H__

#include "main.h"
#include "mesh.h"

// Model struct object definition
struct Model {
    struct Texture *pTextureLoaded;
    int iTextureLoadedLength;
    struct Mesh *pMeshes;
    int iMeshLength;

    char *pDirectory;   // malloc, need free
};

#endif

