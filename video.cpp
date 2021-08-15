#include "video.hpp"

void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "OpenGL Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    #ifdef SET_FULLSCREEN
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    #endif
    if (window == nullptr)
    {
        std::cerr << "Couldn't create Window. ERROR: " << SDL_GetError() << std::endl;
    }
    SDL_GL_SetSwapInterval(0);
    glcontext = SDL_GL_CreateContext(window);
    if (glcontext == nullptr)
    {
        std::cerr << "Couldn't create OpenGL context. ERROR: " << SDL_GetError() << std::endl;
    }
}

void initGL()
{
    glClearColor(bgcol[0], bgcol[1], bgcol[2], 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glewExperimental = GL_TRUE;
    glewInit();
}

void clearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 300.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}