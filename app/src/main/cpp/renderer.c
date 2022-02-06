#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <assimp/cfileio.h>

#include "renderer.h"
#include "main.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "model.h"
#include "mesh.h"
#include "custom_io.h"

#define MODEL_FILE_NAME "backpack/backpack.obj"
#define MODEL_FILE_FORMAT "obj"

void mouse_callback(double xpos, double ypos);

/*
vec3 lightPos = {-1.5f, -0.5f, -2.5f};
vec3 lightAmbient = {0.2f, 0.2f, 0.2f};
vec3 lightDiffuse = {0.8f, 0.8f, 0.8f};
vec3 lightSpecular = {1.0f, 0.9f, 0.75f};
float materialShininess = 64.0f;
*/
int SCREEN_WIDTH = 0, SCREEN_HEIGHT = 0;

/*
int textureNum = 0;
GLuint textures[10];
GLuint floorTexture = 0;
GLuint lightTexture = 0;
GLuint cubeShader = 0, floorShader = 0;
GLuint floorVAO = 0, floorVBO = 0;
GLuint VBO = 0, cubeVAO = 0;
*/
struct Camera *camera = NULL;

float lastX = 400, lastY = 300;
double virtual_xpos = 0.0, virtual_ypox = 0.0;
bool firstMouse = true;
GLuint ourShader = 0;
struct Model model;


int setup(int width, int height)
{
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    virtual_xpos = SCREEN_WIDTH;    // for test
    glViewport(0, 0, width, height);

    // compile shader
    ourShader = load_program("glsl/model_loading.vs.glsl",
                     "glsl/model_loading.fs.glsl");

    // init model
    GE_errorno = init_model(&model, MODEL_FILE_NAME, MODEL_FILE_FORMAT, false);
    GE_CHECK(GE_errorno);

    // camera
    camera = initCamera();

/*
    // old below
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind VAO, (unnecessary)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    textures[0] =  load_texture("res/birch_planks.png", GL_RGB);
    textures[1] =  load_texture("res/bookshell.png", GL_RGBA);
    textures[2] =  load_texture("res/gravel.png", GL_RGB);
    textures[3] =  load_texture("res/netherrack.png", GL_RGB);
    textures[4] =  load_texture("res/dirt.png", GL_RGBA);
    textures[5] =  load_texture("res/birch_trapdoor.png", GL_RGB);
    textures[6] =  load_texture("res/light_blue_concrete.png", GL_RGBA);
    textures[7] =  load_texture("res/purpur_pillar.png", GL_RGBA);
    textures[8] =  load_texture("res/stone_bricks.png", GL_RGB);
    textures[9] =  load_texture("res/bricks.png", GL_RGB);

    floorTexture = load_texture("res/grass_floor.png", GL_RGBA);
    lightTexture = load_texture("res/redstone_lamp_on.png", GL_RGBA);

    shaderSetInt(cubeShader, "texture0", 0);
    shaderSetInt(cubeShader, "texture1", 1);
    glUseProgram(cubeShader);
    shaderSetVec3(cubeShader, "lightPos", lightPos);
    shaderSetInt(cubeShader, "material.diffuse", 0);
    shaderSetFloat(cubeShader, "light.constant", 1.0f);
    shaderSetFloat(cubeShader, "light.linear", 0.09f);
    shaderSetFloat(cubeShader, "light.quadratic", 0.032f);

    glEnable(GL_DEPTH_TEST);

    // floor
    glGenVertexArrays(1, &floorVAO);
    glBindVertexArray(floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices),
                 floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // vec3 materialSpecular = {0.5f, 0.5f, 0.5f};

    glUseProgram(floorShader);
    shaderSetInt(floorShader, "texture0", 0);
    glActiveTexture(GL_TEXTURE0);
*/

    glEnable(GL_DEPTH_TEST);
    return EXIT_SUCCESS;
}

int render()
{
    static float current_frame = 0.0f;
    // render
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*
    // hikari
    glUseProgram(cubeShader);
    shaderSetVec3(cubeShader, "light.position", lightPos);
    shaderSetVec3(cubeShader, "viewPos", camera->position);
    shaderSetVec3(cubeShader, "light.ambient", lightAmbient);
    shaderSetVec3(cubeShader, "light.diffuse", lightDiffuse);
    shaderSetVec3(cubeShader, "light.specular", lightSpecular);

    shaderSetInt(cubeShader,"material.specular", 1);
    shaderSetFloat(cubeShader,"material.shininess", materialShininess);

    // view and projection transform
    mat4 view;
    mat4 projection;
    glm_mat4_identity(view);
    glm_mat4_identity(projection);
    GetViewMatrix(&view);

    glm_perspective(
            glm_rad(camera->zoom),
            (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
            0.1, 100.0, projection
    );

    shaderSetMat4(cubeShader, "view", view[0]);
    shaderSetMat4(cubeShader, "projection", projection[0]);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // render 10 cubes
    for (int i = 0; i < 15; i++)
    {
        // world transform
        mat4 model;
        glm_mat4_identity(model);
        glm_translate(model, cubePositions[i]);
        // rotate cube by time.
        // vec4 axis = {0.0, 1.0f, 0.0};
        // glm_rotate(model,
        // glm_rad(currentFrame * 50 + 20 * i), axis);
        shaderSetMat4(cubeShader, "model", model[0]);
        shaderSetInt(cubeShader, "isLight", false);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
//         glActiveTexture(GL_TEXTURE1);
//         glBindTexture(GL_TEXTURE_2D, textures[textureNum]);

        // render cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // lamp
    mat4 lightModel;
    glm_mat4_identity(lightModel);
    glm_translate(lightModel, lightPos);
    // lamp is smaller than cube
    glm_mat4_scale(lightModel, 0.1);
    shaderSetMat4(cubeShader, "model", lightModel[0]);
    // view and projection
    shaderSetMat4(cubeShader, "view", view[0]);
    shaderSetMat4(cubeShader, "projection", projection[0]);
    shaderSetInt(cubeShader, "isLight", true);
    // render lamp
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lightTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // floor
    glUseProgram(floorShader);
    mat4 rockModel;
    glm_mat4_identity(rockModel);
    vec3 rockPosition = {0.0f, -2.0f, -5.0f};
    glm_translate(rockModel, rockPosition);
    vec4 axis = {1.0, 0.0f, 0.0};
    glm_rotate(rockModel,
               glm_rad(90.0f), axis);
    // glm_mat4_scale(rockModel, 10.0f);
    shaderSetMat4(cubeShader, "model", rockModel[0]);
    shaderSetMat4(cubeShader, "view", view[0]);
    shaderSetMat4(cubeShader, "projection", projection[0]);

    glBindVertexArray(floorVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // camera move
    static double offset = 5.0f;
    virtual_xpos += offset;
    if (virtual_xpos > 2 * SCREEN_WIDTH || virtual_xpos < 0) {
        offset = -offset;
    }
    mouse_callback(virtual_xpos, virtual_ypox);
    */

    glUseProgram(ourShader);
    // view/projection transformations
    // view and projection transform
    mat4 view;
    mat4 projection;
    glm_mat4_identity(view);
    glm_mat4_identity(projection);
    GetViewMatrix(&view);

    glm_perspective(
            glm_rad(camera->zoom),
            (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
            0.1, 100.0, projection
    );

    shaderSetMat4(ourShader, "view", view[0]);
    shaderSetMat4(ourShader, "projection", projection[0]);

    // render the loaded model
    mat4 mat_model;
    glm_mat4_identity(mat_model);
//    vec3 rockPosition = {0.0f, -2.0f, -5.0f};
//    glm_translate(rockModel, rockPosition);
    vec3 model_position = {0.0f, 0.0f, 0.0f};
    glm_translate(mat_model, model_position); // translate it down so it's at the center of the scene
    vec3 model_scale = {0.5f, 0.5f, 0.5f};
    glm_scale(mat_model, model_scale);  // it's a bit too big for our scene, so scale it down
    // rotate cube by time.
    vec4 axis = {0.0f, 1.0f, 0.0f};
    glm_rotate(mat_model,
        glm_rad((float)current_frame * 50 + 20), axis);
    shaderSetMat4(ourShader, "model", (float *) mat_model);
    current_frame += 0.01f;

    // ourModel.Draw(ourShader);
    GE_errorno = draw_model(&model, ourShader);
    GE_CHECK(GE_errorno);

    // unbind
    glBindVertexArray(0);
    return EXIT_SUCCESS;
}

int finish()
{
    // glDeleteVertexArrays(1, &cubeVAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteProgram(cubeShader);
    return EXIT_SUCCESS;
}

int resizeBuffer(int width, int height)
{
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    return 0;
}

void mouse_callback(double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 这里是相反的，因为y坐标从底部往顶部依次增大
    lastX = xpos;
    lastY = ypos;

    ProcessMouseMovement(xoffset, yoffset, true);
}