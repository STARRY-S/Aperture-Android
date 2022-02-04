#include "main.h"
#include "model.h"
#include "cvector.h"

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include <GLES3/gl3.h>
#include <stb_image.h>

int load_model(struct Model *pModel, const char *path);
int process_node(struct Model *pModel, struct aiNode *node, const struct aiScene *scene);
unsigned int texture_from_file(const char *path, const char* directory, bool gamma);
int model_texture_loaded_push_back(struct Model *pModel, struct Texture *pTexture);
struct Mesh *process_mesh(struct Model *pModel, struct aiMesh *mesh, const struct aiScene *scene);
int model_mesh_push_back(struct Model *pModel, struct Mesh *pMesh);

struct Vector *load_material_textures(
    struct Model *pModel, struct aiMaterial *mat, enum aiTextureType type, const char* typeName);

/**
 * Model constructor
 * @param path - file path
 * @param gamma - default to null
 * @return GE_Types
 */
int init_model(struct Model *pModel, const char *path, bool gamma)
{
    if (pModel == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }
    LOGD("BREAK2-1");

    return load_model(pModel, path);
}

int load_model(struct Model *pModel, const char *path)
{
    // TODO: read file from assimp importer
    if (pModel == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }
    LOGD("BREAK2-2");

    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const char* sphere = "s 0 0 0 10";
    const struct aiScene* scene =
            aiImportFileFromMemory(sphere, strlen(sphere),0, ".nff");
    // If the import failed, report it
    if(!scene)
    {
        // TODO: DoTheErrorLogging( aiGetErrorString());
        LOGE("Assimp import failed.");
        return GE_ERROR_ASSIMP_IMPORT_FAILED;
    }
    LOGD("BREAK2-3");

    // Now we can access the file's contents
    // TODO: Do the process scene things..
    process_node(pModel, scene->mRootNode, scene);
    LOGD("BREAK2-4");

    // We're done. Release all resources associated with this import
    aiReleaseImport(scene);
    LOGD("BREAK2-5");

    return GE_ERROR_SUCCESS;
}

int process_node(struct Model *pModel, struct aiNode *node, const struct aiScene *scene)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        pModel->iMeshLength++;
        struct Mesh *pNewMesh = process_mesh(pModel, mesh, scene);
        model_mesh_push_back(pModel, pNewMesh);
        free(pNewMesh);
        pNewMesh = NULL;
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(pModel, node->mChildren[i], scene);
    }

    return 0;
}

/**
 * Process mesh,
 * @param pModel
 * @param mesh
 * @param scene
 * @return new mesh struct object, by malloc, need free
 */
struct Mesh *process_mesh(struct Model *pModel, struct aiMesh *mesh, const struct aiScene *scene)
{
    if (pModel == NULL || mesh == NULL || scene == NULL) {
        return NULL;
    }
    // data to fill
    struct Vector vecVertices;
    struct Vector vecIndices;
    struct Vector vecTextures;

    init_vector(&vecVertices, GE_VECTOR_VERTEX);
    init_vector(&vecIndices, GE_VECTOR_UINT);
    init_vector(&vecTextures, GE_VECTOR_TEXTURE);

//    vector<Vertex> vertices;
//    vector<unsigned int> indices;
//    vector<Texture> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        struct Vertex vertex;
        vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector[0] = mesh->mVertices[i].x;
        vector[1] = mesh->mVertices[i].y;
        vector[2] = mesh->mVertices[i].z;
        vertex.Position[0] = vector[0];
        vertex.Position[1] = vector[1];
        vertex.Position[2] = vector[2];

        // normals
        if (mesh->mNormals != NULL)
        {
            vector[0] = mesh->mNormals[i].x;
            vector[1] = mesh->mNormals[i].y;
            vector[2] = mesh->mNormals[i].z;
            vertex.Normal[0] = vector[0];
            vertex.Normal[1] = vector[1];
            vertex.Normal[2] = vector[2];
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vec2 vec;
            // a vertex can contain up to 8 different texture coordinates.
            // We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates
            // so we always take the first set (0).
            vec[0] = mesh->mTextureCoords[0][i].x;
            vec[1] = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords[0] = vec[0];
            vertex.TexCoords[1] = vec[1];

            // tangent
            vector[0] = mesh->mTangents[i].x;
            vector[1] = mesh->mTangents[i].y;
            vector[2] = mesh->mTangents[i].z;
            vertex.Tangent[0] = vector[0];
            vertex.Tangent[1] = vector[1];
            vertex.Tangent[2] = vector[2];

            // bitangent
            vector[0] = mesh->mBitangents[i].x;
            vector[1] = mesh->mBitangents[i].y;
            vector[2] = mesh->mBitangents[i].z;
            vertex.BigTangent[0] = vector[0];
            vertex.BigTangent[1] = vector[1];
            vertex.BigTangent[2] = vector[2];

        }
        else {
            vertex.TexCoords[0] = 0.0f;
            vertex.TexCoords[1] = 0.0f;
        }

        // vertices.push_back(vertex);
        vector_push_back(&vecVertices, (const char *) &vertex);
    }
    // now wak through each of the mesh's faces
    // (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        struct aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            // indices.push_back(face.mIndices[j]);
            vector_push_back(&vecIndices, (const char *) &face.mIndices[j]);
        }
    }
    // process materials
    struct aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    struct Vector *pVecDiffuseMaps;
    size_t size = sizeof(struct Texture);
    pVecDiffuseMaps = load_material_textures(
            pModel, material, aiTextureType_DIFFUSE, "texture_diffuse");
    vector_insert_back(&vecTextures, pVecDiffuseMaps->data, size * pVecDiffuseMaps->length);

    // 2. specular maps
    struct Vector *pVecSpecularMaps;
    pVecSpecularMaps = load_material_textures(
            pModel, material, aiTextureType_SPECULAR, "texture_specular"
            );
    vector_insert_back(&vecTextures, pVecSpecularMaps->data, size * pVecSpecularMaps->length);

    // 3. normal maps
    struct Vector *pVecNormalMaps;
    pVecNormalMaps = load_material_textures(
            pModel, material, aiTextureType_HEIGHT, "texture_normal"
            );
    vector_insert_back(&vecTextures, pVecNormalMaps->data, size * pVecNormalMaps->length);

    // 4. height maps
    struct Vector *pVecHeightMaps;
    pVecHeightMaps = load_material_textures(
            pModel, material, aiTextureType_AMBIENT, "texture_height"
        );
    vector_insert_back(&vecTextures, pVecHeightMaps->data, size * pVecHeightMaps->length);

    // return a mesh object created from the extracted mesh data
    struct Mesh *pNewMesh = malloc(sizeof (struct Mesh));
    if (pNewMesh == NULL) {
        return NULL;
    }
    init_mesh(pNewMesh, (struct Vertex *) vecVertices.data, vecVertices.length,
              (unsigned int *) vecIndices.data, vecIndices.length,
              (struct Texture *) vecTextures.data, vecTextures.length);
    return pNewMesh;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
struct Vector *load_material_textures(struct Model *pModel,
        struct aiMaterial *mat, enum aiTextureType type, const char* typeName)
{
    struct Vector *pVecTextures = malloc(sizeof(struct Vector));
    if (pVecTextures == NULL) {
        return NULL;
    }

    init_vector(pVecTextures, GE_VECTOR_TEXTURE);
    // vector<Texture> textures;
    // for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    for(unsigned int i = 0; i < aiGetMaterialTextureCount(mat, type); i++)
    {
        struct aiString str;
        // mat->GetTexture(type, i, &str);
        aiGetMaterialTexture(mat, type, i, &str,
             NULL, NULL, NULL, NULL, NULL, NULL);
        // check if texture was loaded before and if so,
        // continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < pModel->iTextureLoadedLength; j++)
        {
            if(strcmp(pModel->pTextureLoaded[j].path, str.data) == 0)
            {
                vector_push_back(pVecTextures, (const char *) &pModel->pTextureLoaded[j]);
                // textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded,
                             // continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {
            // if texture hasn't been loaded already, load it
            struct Texture texture;
            texture.id = texture_from_file(str.data, pModel->pDirectory, false);

            texture_set_path(&texture, str.data);
            texture_set_type(&texture, typeName);
            // texture.type = typeName;
            // texture.path = str.data();

            // textures.push_back(texture);
            // textures_loaded.push_back(texture);
            vector_push_back(pVecTextures, (const char*) &texture);
            // store it as texture loaded for entire model,
            // to ensure we won't unnecessary load duplicate textures.
            model_texture_loaded_push_back(pModel, &texture);
        }
    }
    return pVecTextures;
}

int model_texture_loaded_push_back(struct Model *pModel, struct Texture *pTexture)
{
    if (pModel == NULL || pTexture == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }
    if (pModel->pTextureLoaded == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }

    // add a new texture struct object into model
    pModel->pTextureLoaded = realloc(pModel->pTextureLoaded,
                     sizeof(struct Texture) * (pModel->iTextureLoadedLength + 1));
    if (pModel->pTextureLoaded == NULL) {
        LOGE("Realloc error.");
        return GE_ERROR_MALLOC_FAILED;
    }
    pModel->iTextureLoadedLength++;
    struct Texture *pNewTexture = pModel->pTextureLoaded + pModel->iTextureLoadedLength;
    texture_set_type(pNewTexture, pTexture->type);
    texture_set_path(pNewTexture, pTexture->path);
    pNewTexture->id = pTexture->id;

    return 0;
}

int model_mesh_push_back(struct Model *pModel, struct Mesh *pMesh)
{
    if (pModel == NULL || pMesh == NULL) {
        return GE_ERROR_INVALID_POINTER;
    }

    // add a new mesh struct object into model
    pModel->pMeshes = realloc(pModel->pMeshes,
                              sizeof(struct Mesh) * (pModel->iMeshLength + 1));
    if (pModel->pMeshes == NULL) {
        LOGE("Realloc error.");
        return GE_ERROR_MALLOC_FAILED;
    }
    pModel->iMeshLength++;
    struct Mesh *pNewMesh = pModel->pMeshes + pModel->iMeshLength;
    // copy old mesh data memory to new mesh
    copy_mesh(pNewMesh, pMesh);

    return 0;
}

/**
 * Load texture from file.
 * @param path
 * @param directory
 * @param gamma
 * @return
 */
unsigned int texture_from_file(const char *path, const char *directory, bool gamma)
{
    /*
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
    GLenum format;
    if (nrComponents == 1)
    format = GL_RED;
    else if (nrComponents == 3)
    format = GL_RGB;
    else if (nrComponents == 4)
    format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    }
    else
    {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
    }
    */
    return 0;
}

int draw_model(struct Model *pModel, unsigned int shader)
{
    return 0;
}