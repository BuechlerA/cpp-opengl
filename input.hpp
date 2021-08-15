#include "commons.hpp"

extern Input myInputs;

extern float deltaTime;
extern float lastX;
extern float lastY;
extern float yaw;
extern float pitch;
extern float fov;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern glm::vec3 cameraPos;

extern int lastWheel;

extern bool closemsg;

void checkInputs();
void OnKeyDown(SDL_KeyboardEvent *event);
void OnKeyUp(SDL_KeyboardEvent *event);
void OnMouseMove();
void OnMouseButtonDown(SDL_MouseButtonEvent *event);
void OnMouseButtonUp(SDL_MouseButtonEvent *event);
void OnMouseWheel(SDL_MouseWheelEvent *event);
void moveCam();