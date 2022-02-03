#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#include <GLES3/gl3.h>

#include "mesh.h"
#include "main.h"
#include "shader.h"

// private method
int setupMesh(struct Mesh *pMesh);
bool isValidMesh(struct Mesh *pMesh);

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
int initMesh(struct Mesh *pMesh, struct Vertex* pVertices, int iVerticesLength,
        unsigned int *pIndices, int iIndicesLength,
        struct Texture *pTexture, int iTextureLength)
{
    if (!pMesh || !pVertices || !pIndices || !pTexture) {
        return GE_ERROR_INVALID_POINTER;
    }

    if (iVerticesLength <= 0 || iIndicesLength <= 0 || iTextureLength <= 0) {
        return GE_ERROR_INVALID_PARAMETER;
    }

    struct Vertex *pNewVertex = NULL;
    pNewVertex = (struct Vertex *) malloc(sizeof(struct Vertex) * iVerticesLength);
    if (pNewVertex == NULL) {
        return GE_ERROR_MALLOC_FAILED;
    }
    memcpy(pNewVertex, pVertices, sizeof(struct Vertex) * iVerticesLength);
    pMesh->pVertices = pNewVertex;

    unsigned int *pNewIndices = NULL;
    pNewIndices = (unsigned int *) malloc(sizeof(unsigned int) * iIndicesLength);
    if (pNewIndices == NULL) {
        return GE_ERROR_MALLOC_FAILED;
    }
    memcpy(pNewIndices, pIndices, sizeof(unsigned int ) * iIndicesLength);
    pMesh->pIndices = pNewIndices;

    struct Texture *pNewTexture = NULL;
    pNewTexture = (struct Texture *) malloc(sizeof(struct Texture) * iTextureLength);
    if (pNewTexture == NULL) {
        return GE_ERROR_MALLOC_FAILED;
    }
    memcpy(pNewTexture, pTexture, sizeof(struct Texture) * iTextureLength);
    pMesh->pTextures = pNewTexture;

    setupMesh(pMesh);
    return 0;
}

/**
 * Free mesh struct object
 * @param pMesh
 * @return 0 if success
 */
int freeMesh(struct Mesh *pMesh)
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
 * private, setup mesh
 * @param pMesh
 * @return GE_Types
 */
int setupMesh(struct Mesh *pMesh)
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
bool isValidMesh(struct Mesh *pMesh)
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
 * @param pMesh
 * @param shader
 */
int drawMesh(struct Mesh *pMesh, unsigned int shader)
{
    if (!isValidMesh(pMesh)) {
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