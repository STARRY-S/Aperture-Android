#include "camera.h"

struct Camera Camera;

struct Camera* initCamera()
{
    // vec3 cameraPos = { 0.0f, 0.0f, 3.0f };
    Camera.position[0] = 0.0f;
    Camera.position[1] = 0.0f;
    Camera.position[2] = 6.0f;

    // vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
    Camera.front[0] = 0.0f;
    Camera.front[1] = 0.0f;
    Camera.front[2] = -1.0f;

    // vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    Camera.up[0] = 0.0f;
    Camera.up[1] = 1.0f;
    Camera.up[2] = 0.0f;

    // float yaw = -90.0f;
    Camera.yaw = -90.0f;
    // float pitch = 0.0f;
    Camera.pitch = 0.0f;

    Camera.mSpeed = 2.5f;
    Camera.mSensitivity = 0.05f;
    Camera.zoom = 45.0f;

    return &Camera;
}

void updateCameraVectors()
{
    float sin_yaw = sin(glm_rad(Camera.yaw));
    float cos_yaw = cos(glm_rad(Camera.yaw));
    float sin_pitch = sin(glm_rad(Camera.pitch));
    float cos_pitch = cos(glm_rad(Camera.pitch));

    Camera.front[0] = cos_yaw * cos_pitch;
    Camera.front[1] = sin_pitch;
    Camera.front[2] = sin_yaw * cos_pitch;
    glm_vec3_normalize(Camera.front);
}

struct Camera* getCamera()
{
    return &Camera;
}


void GetViewMatrix(mat4 *temp)
{
    vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
    glm_vec3_add(Camera.position, Camera.front, cameraTarget);
    glm_lookat(Camera.position, cameraTarget, Camera.up, *temp);
}

void ProcessKeyboard(int direction, float deltaTime)
{
    float velocity = Camera.mSpeed * deltaTime;
    vec3 temp = {0.0f, 0.0f, 0.0f};
    switch (direction)
    {
        case CAM_MOV_FORWARD:
        {
            glm_vec3_scale(Camera.front, velocity, temp);
            glm_vec3_add(Camera.position, temp, Camera.position);
            break;
        }
        case CAM_MOV_BACKWARD:
        {
            glm_vec3_scale(Camera.front, velocity, temp);
            glm_vec3_sub(Camera.position, temp, Camera.position);
            break;
        }
        case CAM_MOV_LEFT:
        {
            glm_vec3_cross(Camera.front, Camera.up, temp);
            glm_vec3_normalize(temp);
            glm_vec3_scale(temp, velocity, temp);
            glm_vec3_sub(Camera.position, temp, Camera.position);
            break;
        }
        case CAM_MOV_RIGHT:
        {
            glm_vec3_cross(Camera.front, Camera.up, temp);
            glm_vec3_normalize(temp);
            glm_vec3_scale(temp, velocity, temp);
            glm_vec3_add(Camera.position, temp, Camera.position);
            break;
        }
        case CAM_MOV_UP:
        {
            glm_vec3_scale(Camera.up, velocity, temp);
            glm_vec3_add(Camera.position, temp, Camera.position);
            break;
        }
        case CAM_MOV_DOWN:
        {
            glm_vec3_scale(Camera.up, velocity, temp);
            glm_vec3_sub(Camera.position, temp, Camera.position);
            break;
        }
        default:
            break;
    }

}

void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= Camera.mSensitivity;
    yoffset *= Camera.mSensitivity;

    Camera.yaw += xoffset;
    Camera.pitch += yoffset;

    if (constrainPitch) {
        if(Camera.pitch > 89.0f)
            Camera.pitch =  89.0f;
        if(Camera.pitch < -89.0f)
            Camera.pitch = -89.0f;
    }

    updateCameraVectors();
}

void ProcessMouseScroll(float yoffset)
{
    Camera.zoom -= yoffset;
    if (Camera.zoom < 1.0f) {
        Camera.zoom = 1.0f;
    } else if (Camera.zoom > 45.0f) {
        Camera.zoom = 45.0f;
    }
}

void ProcessMoveBlock(vec3 blockPos)
{
    // glm_vec3_distance(blockPos);
}
