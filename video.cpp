#include "video.hpp"

void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);

    // Request OpenGL 3.3 Core Profile for macOS compatibility
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    // Optional but recommended attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
        "OpenGL Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI  // High DPI support for Retina displays
    );
    #ifdef SET_FULLSCREEN
        SDL_SetWindowFullscreen(window, SDL_TRUE);
    #endif
    if (window == nullptr)
    {
        std::cerr << "Couldn't create Window. ERROR: " << SDL_GetError() << std::endl;
        exit(1);
    }

    glcontext = SDL_GL_CreateContext(window);
    if (glcontext == nullptr)
    {
        std::cerr << "Couldn't create OpenGL context. ERROR: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetSwapInterval(0);
}

void initGL()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // Print OpenGL version for debugging
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glClearColor(bgcol[0], bgcol[1], bgcol[2], 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Removed: glShadeModel(GL_SMOOTH) - not available in Core Profile
    // Removed: glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST) - ignored in Core Profile
    glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void clearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Removed legacy fixed-function pipeline code:
    // - glMatrixMode(), glLoadIdentity(), gluPerspective()
    // Modern pipeline handles all transformations via shaders and uniforms
}