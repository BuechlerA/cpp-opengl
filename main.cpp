#include "main.hpp"
#include "shader.hpp"

SDL_Window *window;
SDL_GLContext glcontext;
Shader shader;
Shader lightShader;

glm::vec3 cameraPos = {0.0f, 0.0f, 3.0f};
glm::vec3 cameraFront = {0.0f, 0.0f, -1.0f};
glm::vec3 cameraUp = {0.0f, 1.0f, 0.0f};

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = WINDOW_WIDTH/2;
float lastY = WINDOW_HEIGHT/2;
int lastWheel = 0;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

float bgcol[3] = { 0.05f, 0.05f, 0.05f};

bool closemsg = false;

void cleanUp();

int main(int argc, char *argv[])
{
    initSDL();
    initGL();

    shader.Build("shaders/vertex.glsl", "shaders/lightingFrag.glsl");
    lightShader.Build("shaders/vertex.glsl", "shaders/lightCubeFrag.glsl");
    initRender();
    initGui();

    atexit(cleanUp);

    while (!closemsg)
    {
        setDeltaTime();

        checkInputs();
        
        drawGui();
    
        glClearColor(bgcol[0], bgcol[1], bgcol[2], 1.0f);

        moveCam();

        clearWindow();

        doRender();

        guiRender();

        SDL_GL_SwapWindow(window);
    }
    
    cleanUp();
    return 0;
}

void setDeltaTime()
{
    float currentFrame = SDL_GetTicks();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void cleanUp()
{
    cleanRenderObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(glcontext);
    SDL_Quit();
}