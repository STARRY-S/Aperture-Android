#ifndef __MODEL_H__
#define __MODEL_H__

#include "main.h"
#include "mesh.h"
#include "cvector.h"

// Model struct object definition
struct Model {
    struct Texture *pTextureLoaded;
    int iTextureLoadedLength;
    struct Mesh *pMeshes;
    int iMeshLength;

//    struct Vector *pVecTextureLoaded;
//    struct Vector *pVecMeshes;

    char *pDirectory;   // malloc, need free
};

int init_model(struct Model *pModel, const char *path, const char *format, bool gamma);
int draw_model(struct Model *pModel, unsigned int shader);

#endif

