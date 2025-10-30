#include <iostream>

#include "structs.hpp"

#define GLEW_STATIC

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Platform-specific OpenGL headers
#ifdef __APPLE__
    #include <OpenGL/glew.h>    // macOS path
    #include <OpenGL/gl.h>      // macOS OpenGL
    // GLU removed - deprecated on macOS and not needed
#else
    #include <GL/glew.h>        // Windows/Linux
    #include <GL/gl.h>
    // GLU removed - not used in Core Profile
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
//#define SET_FULLSCREEN