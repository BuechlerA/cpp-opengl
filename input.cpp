#include "input.hpp"

void checkInputs()
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            //exit(0);
            closemsg = true;
            break;
        case SDL_KEYDOWN:
            if (io.WantCaptureKeyboard)
            {
                return;
            }
            else
            {
                OnKeyDown(&event.key);
            }
            break;
        case SDL_KEYUP:
            OnKeyUp(&event.key);
            break;
        case SDL_MOUSEMOTION:
            if (io.WantCaptureMouse)
            {
                return;
            }
            else
            {
                OnMouseMove();
            }
            break;
        case SDL_MOUSEWHEEL:
            if (io.WantCaptureMouse)
            {
                return;
            }
            else
            {
                OnMouseWheel(&event.wheel);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            OnMouseButtonDown(&event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            OnMouseButtonUp(&event.button);
            break;
        default:
            break;
        }
    }
}

void OnKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0)
    {
        if (event->keysym.scancode == SDL_SCANCODE_W)
        {
            myInputs.forward = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_S)
        {
            myInputs.back = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_A)
        {
            myInputs.left = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_D)
        {
            myInputs.right = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_SPACE)
        {
            myInputs.up = 1;
        }
        if (event->keysym.scancode == SDL_SCANCODE_LCTRL)
        {
            myInputs.down = 1;
        }
    }
}

void OnKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0)
    {
        if (event->keysym.scancode == SDL_SCANCODE_W)
        {
            myInputs.forward = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_S)
        {
            myInputs.back = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_A)
        {
            myInputs.left = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_D)
        {
            myInputs.right = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_SPACE)
        {
            myInputs.up = 0;
        }
        if (event->keysym.scancode == SDL_SCANCODE_LCTRL)
        {
            myInputs.down = 0;
        }
    }
}

void OnMouseMove()
{
    int xpos, ypos;
    SDL_GetMouseState(&xpos, &ypos);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 frontVec;
    frontVec[0] = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontVec[1] = sin(glm::radians(pitch));
    frontVec[2] = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glm::normalize(frontVec);
    cameraFront = frontVec;
}

void OnMouseButtonDown(SDL_MouseButtonEvent *event)
{
    if (event->button == SDL_BUTTON_LEFT)
    {
        myInputs.mouse_left = 1;
    }
    if (event->button == SDL_BUTTON_RIGHT)
    {
        myInputs.mouse_right = 1;
    }
}

void OnMouseButtonUp(SDL_MouseButtonEvent *event)
{
    if (event->button == SDL_BUTTON_LEFT)
    {
        myInputs.mouse_left = 0;
    }
    if (event->button == SDL_BUTTON_RIGHT)
    {
        myInputs.mouse_right = 0;
    }
}

void OnMouseWheel(SDL_MouseWheelEvent *event)
{
    int wheel = event->y;
    int wheeloffset = wheel - lastWheel;
    lastWheel = wheel;

    if (wheel > 0)
    {
        fov -= wheeloffset;
        if (fov < 0.1f)
        {
            fov = 0.1f;
        }
    }
    else if (wheel < 0)
    {
        fov += -wheeloffset;
        if (fov > 45.0f)
        {
            fov = 45.0f;
        }
    }
}

void moveCam()
{
    const float cameraSpeed = 0.005f * deltaTime;
    glm::vec3 crossProd;
    if (myInputs.forward == 1)
    {
        cameraPos += cameraFront * cameraSpeed;
    }
    if (myInputs.back == 1)
    {
        cameraPos += cameraFront * -cameraSpeed;
    }
    if (myInputs.left == 1)
    {
        crossProd = glm::cross(cameraFront, cameraUp);
        glm::normalize(crossProd);
        cameraPos += crossProd * -cameraSpeed;
    }
    if (myInputs.right == 1)
    {
        crossProd = glm::cross(cameraFront, cameraUp);
        glm::normalize(crossProd);
        cameraPos += crossProd * cameraSpeed;
    }
    if (myInputs.up == 1)
    {
        cameraPos += cameraUp * cameraSpeed;
    }
    if (myInputs.down == 1)
    {
        cameraPos += cameraUp * -cameraSpeed;
    }
}