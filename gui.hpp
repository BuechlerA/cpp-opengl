#include "commons.hpp"
#include "shader.hpp"

extern SDL_Window *window;
extern SDL_GLContext glcontext;
extern Shader shader;

extern float bgcol[3];
extern float lightCol[3];
extern float lightPos[3];
extern float lightAttenuation[3];

extern bool pointLightActive;
extern bool isOrbitLight;
extern bool closemsg;

void initGui();
void drawGui();
void guiRender();