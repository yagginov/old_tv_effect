#include "camera.h"

Camera::Camera(glm::vec3 p_position, glm::vec3 up, float yaw, float pitch) : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), 
    MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    default_position = p_position;
    position = p_position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, 
    float upX, float upY, float upZ, 
    float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    default_position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    glm::vec3 front = Front;
    front.y = 0.0f;
    front = glm::normalize(front);
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
    {
        default_position += front * velocity;
    }      
    if (direction == BACKWARD)
    {
        default_position -= front * velocity;
    }        
    if (direction == LEFT) 
    {
        default_position -= Right * velocity;
    }       
    if (direction == RIGHT) 
    {
        default_position += Right * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f) 
        {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) 
        {
            Pitch = -89.0f;
        }
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
    {
        Zoom -= yoffset;
    }
    if (Zoom <= 1.0f)
    {
        Zoom = 1.0f;
    }
    if (Zoom >= 45.0f) 
    {
        Zoom = 45.0f;
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));  
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::add_position(glm::vec3 pos)
{
    position += pos;
}

void Camera::reset()
{
    position = default_position;
    Zoom = 45.0f;
}

glm::vec3 Camera::get_pos() const
{
    return position;
}