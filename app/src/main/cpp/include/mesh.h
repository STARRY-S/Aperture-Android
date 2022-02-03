#ifndef __MESH_H__
#include "main.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex { // points
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;

    vec3 Tangent;
    vec3 BigTangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    char *type; // malloc, need free
    char *path; // malloc, need free
};

struct Mesh {
    // 网格数据
    struct Vertex* pVertices;   // array, use malloc, need free.
    int iVerticesLength;
    unsigned int *pIndices;     // array, use malloc, need free.
    int iIndicesLength;
    struct Texture *pTextures;   // array, use malloc, need free.
    int iTextureLength;
    // 渲染数据
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

int initMesh(struct Mesh *pMesh, struct Vertex* pVertices, int iVerticesLength,
             unsigned int *pIndices, int iIndicesLength,
             struct Texture *pTexture, int iTextureLength);
int freeMesh(struct Mesh *pMesh);
int drawMesh(struct Mesh *pMesh, unsigned int shader);

#endif // __MESH_H__