#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#include <GLES3/gl3.h>

#include "mesh.h"
#include "main.h"
#include "shader.h"

// private method
int setup_mesh(struct Mesh *pMesh);
bool is_valid_mesh(struct Mesh *pMesh);

/**
 * Mesh Constructor
 * @param pMesh
 * @param pVertices
 * @param iVerticesLength
 * @param pIndices
 * @param iIndicesLength
 * @param pTexture
 * @param iTextureLength
 * @return - GE_Types
 */
int init_mesh(struct Mesh *pMesh, struct Vertex* pVertices, int iVerticesLength,
        unsigned int *pIndices, int iIndicesLength,
        struct Texture *pTexture, int iTextureLength)
{
    if (!pMesh || !pVertices || !pIndices || !pTexture) {
        return GE_ERROR_INVALID_POINTER;
    }

    memset(pMesh, 0, sizeof(struct Mesh));

    struct Vertex *pNewVertex = NULL;
    if (iVerticesLength > 0) {
        pNewVertex = (struct Vertex *) malloc(sizeof(struct Vertex) * iVerticesLength);
        if (pNewVertex == NULL) {
            return GE_ERROR_MALLOC_FAILED;
        }
        memcpy(pNewVertex, pVertices, sizeof(struct Vertex) * iVerticesLength);
    }
    pMesh->iVerticesLength = iVerticesLength;
    pMesh->pVertices = pNewVertex;

    unsigned int *pNewIndices = NULL;
    if (iIndicesLength > 0) {
        pNewIndices = (unsigned int *) malloc(sizeof(unsigned int) * iIndicesLength);
        if (pNewIndices == NULL) {
            return GE_ERROR_MALLOC_FAILED;
        }
        memcpy(pNewIndices, pIndices, sizeof(unsigned int) * iIndicesLength);
    }
    pMesh->iIndicesLength = iIndicesLength;
    pMesh->pIndices = pNewIndices;

    struct Texture *pNewTexture = NULL;
    if (iTextureLength > 0) {
        pNewTexture = (struct Texture *) malloc(sizeof(struct Texture) * iTextureLength);
        if (pNewTexture == NULL) {
            return GE_ERROR_MALLOC_FAILED;
        }
        memcpy(pNewTexture, pTexture, sizeof(struct Texture) * iTextureLength);
    }
    pMesh->iTextureLength = iTextureLength;
    pMesh->pTextures = pNewTexture;

    setup_mesh(pMesh);
    return 0;
}

/**
 * Free mesh struct object
 * @param pMesh
 * @return 0 if success
 */
int free_mesh(struct Mesh *pMesh)
{
    if (pMesh == NULL) {
        return 0;
    }
    if (pMesh->pIndices) {
        free(pMesh->pIndices);
        pMesh->pIndices = NULL;
    }

    if (pMesh->pVertices) {
        free(pMesh->pVertices);
        pMesh->pVertices = NULL;
    }

    if (pMesh->pTextures) {
        free(pMesh->pTextures);
        pMesh->pTextures = NULL;
    }

    free(pMesh);
    pMesh = NULL;
    return 0;
}

/**
 * Copy mesh
 * @param pNewMesh
 * @param pOldMesh
 * @return GE_Types
 */
int copy_mesh(struct Mesh *pNewMesh, struct Mesh *pOldMesh)
{
    if (pNewMesh == NULL || pOldMesh == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }
    LOGD("Start copy mesh.");

//    if (pNewMesh->pTextures != NULL) {
//        free(pNewMesh->pTextures);
//        pNewMesh->pTextures = NULL;
//        LOGD("Free ed old mesh Textures.");
//    }
    pNewMesh->iTextureLength = pOldMesh->iTextureLength;
    if (pOldMesh->iTextureLength > 0) {
        pNewMesh->pTextures = malloc(pNewMesh->iTextureLength * sizeof(struct Texture));
        memcpy(pNewMesh->pTextures, pOldMesh->pTextures,
               pNewMesh->iTextureLength * sizeof(struct Texture));
    }

//    if (pNewMesh->pIndices != NULL) {
//        free(pNewMesh->pIndices);
//        pNewMesh->pIndices = NULL;
//    }
    pNewMesh->iIndicesLength = pOldMesh->iIndicesLength;
    if (pOldMesh->iIndicesLength > 0) {
        pNewMesh->pIndices = malloc(pNewMesh->iIndicesLength * sizeof(unsigned int));
        memcpy(pNewMesh->pIndices, pOldMesh->pIndices,
               pNewMesh->iIndicesLength * sizeof(unsigned int));
    }

//    if (pNewMesh->pVertices != NULL) {
//        free(pNewMesh->pVertices);
//        pNewMesh->pVertices = NULL;
//    }
    pNewMesh->iVerticesLength = pOldMesh->iVerticesLength;
    if (pOldMesh->iVerticesLength > 0) {
        pNewMesh->pVertices = malloc(pNewMesh->iVerticesLength * sizeof(struct Vertex));
        memcpy(pNewMesh->pVertices, pOldMesh->pVertices,
               pNewMesh->iVerticesLength * sizeof(struct Vertex));
    }

    pNewMesh->VAO = pOldMesh->VAO;
    pNewMesh->VBO = pOldMesh->VBO;
    pNewMesh->EBO = pOldMesh->EBO;
    LOGD("Finish copy mesh.");

    return 0;
}

/**
 * private, setup mesh, generate GL buffers.
 * @param pMesh
 * @return GE_Types
 */
int setup_mesh(struct Mesh *pMesh)
{
    if (!pMesh) {
        return GE_ERROR_INVALID_POINTER;
    }
    glGenVertexArrays(1, &pMesh->VAO);
    glGenBuffers(1, &pMesh->VBO);
    glGenBuffers(1, &pMesh->EBO);

    glBindVertexArray(pMesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, pMesh->VBO);

    glBufferData(GL_ARRAY_BUFFER,pMesh->iVerticesLength * sizeof(struct Vertex),
            pMesh->pVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->iIndicesLength * sizeof(unsigned int),
                 pMesh->pIndices, GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(struct Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(struct Vertex), (void*)offsetof(struct Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof(struct Vertex), (void*)offsetof(struct Vertex, TexCoords));

    glBindVertexArray(0);
    return 0;
}

/**
 * Check whether Mesh struct object is initialized or not
 * @param pMesh
 * @return true: initialized, false: not initialized
 */
bool is_valid_mesh(struct Mesh *pMesh)
{
    if (pMesh == NULL) {
        return false;
    }

    if (!pMesh->pIndices || !pMesh ->pVertices || !pMesh->pTextures)
    {
        return false;
    }

    if (pMesh->iIndicesLength <= 0  ||
        pMesh->iVerticesLength <= 0 ||
        pMesh->iTextureLength <= 0)
    {
        return false;
    }

    return true;
}

/**
 * Draw Mesh
 * @param pMesh - the pointer to the mesh
 * @param shader - shader id
 * @return GE_Types
 */
int draw_mesh(struct Mesh *pMesh, unsigned int shader)
{
    if (!is_valid_mesh(pMesh)) {
        return GE_ERROR_MESH_UNINITIALIZED;
    }

    // bind appropriate texture
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < pMesh->iTextureLength; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture before binding
        // retrieve texture number
        char sNumber[32];
        const char *spName = pMesh->pTextures[i].type;
        if (strcmp(spName, "texture_diffuse") == 0)
            sprintf(sNumber, "%u", diffuseNr++);
        else if (strcmp(spName, "texture_specular") == 0)
            sprintf(sNumber, "%u", specularNr++);
        else if (strcmp(spName, "texture_normal") == 0)
            sprintf(sNumber, "%u", normalNr++);
        else if (strcmp(spName, "texture_height") == 0)
            sprintf(sNumber, "%u", heightNr++);

        char buffer[128];
        sprintf(buffer, "%s%s", spName, sNumber);
        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader, buffer), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, pMesh->pTextures[i].id);
    }

    // draw mesh
    glBindVertexArray(pMesh->VAO);
    glDrawElements(GL_TRIANGLES, pMesh->iIndicesLength, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured :)
    glActiveTexture(GL_TEXTURE0);
    return GE_ERROR_SUCCESS;
}

int texture_set_type(struct Texture *pTexture, const char *typeName)
{
    if (pTexture == NULL || typeName == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }

    if (pTexture->type != NULL) {
        free(pTexture->type);
        pTexture->type = NULL;
    }
    pTexture->type = malloc(sizeof(char) * (strlen(typeName) + 1) );
    strcpy(pTexture->type, typeName);
    return 0;
}

int texture_set_path(struct Texture *pTexture, const char *pathName)
{
    if (pTexture == NULL || pathName == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }

    if (pTexture->path != NULL) {
        free(pTexture->path);
        pTexture->path = NULL;
    }
    pTexture->path = malloc(sizeof(char) * (strlen(pathName) + 1) );
    strcpy(pTexture->path, pathName);
    return 0;
}