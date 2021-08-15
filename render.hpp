#include "commons.hpp"
#include "shader.hpp"
#include "shader_compiler.hpp"

extern float fov;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern Shader shader;
extern Shader lightShader;

void initRender();
void doRender();
void cleanRenderObjects();