#ifndef GAMEENGINE_CAMERA_H
#define GAMEENGINE_CAMERA_H

#include <GLES3/gl3.h>
#include <stdio.h>
#include <stdbool.h>

#include "cglm/cglm.h"
#include "cglm/call.h"

enum Camera_Movement {
    CAM_MOV_FORWARD,
    CAM_MOV_BACKWARD,
    CAM_MOV_LEFT,
    CAM_MOV_RIGHT,
    CAM_MOV_UP,
    CAM_MOV_DOWN
};

struct Camera {
    // camera Attributes
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldup;

    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float mSpeed;           // movement speed
    float mSensitivity;     // mouse sensitivity
    float zoom;
};

struct Camera* initCamera();
struct Camera* getCamera();
void updateCameraVectors();
void GetViewMatrix(mat4 *temp);
void ProcessKeyboard(int direction, float deltaTime);
void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
void ProcessMouseScroll(float yoffset);

#endif //GAMEENGINE_CAMERA_H
