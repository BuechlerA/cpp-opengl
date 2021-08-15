#include "render.hpp"
#include "objects.hpp"

GLuint vao;
GLuint vbo;
GLuint ebo;

GLuint lightVao;

glm::vec3 lightCol = {1.0f, 1.0f, 1.0f};
glm::vec3 lightPos = {1.0f, 1.0f, -2.0f};
glm::vec3 lightDir = {5.0f, 5.0f, -5.0f};

float lightAttenuation[3] = {1.0f, 0.09f, 0.032f};

bool pointLightActive = false;
bool isOrbitLight = false;

void initRender()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLint pos = glGetAttribLocation(lightShader.shaderID, "pos");
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);;
    glEnableVertexAttribArray(pos);
    
    shader.Bind();

    shader.SetFragDataLocation(0, "outColor");
    lightShader.SetFragDataLocation(0, "FragColor");

    shader.SetVertexAttribLocation("pos", 3, GL_FLOAT, GL_FALSE, 11*(sizeof(float)), 0);
    shader.SetVertexAttribLocation("texcoord", 2, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(3*sizeof(float)));
    shader.SetVertexAttribLocation("color", 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(5*sizeof(float)));
    shader.SetVertexAttribLocation("normal", 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(8*sizeof(float)));

    lightShader.SetVertexAttribLocation("pos", 3, GL_FLOAT, GL_FALSE, 11*(sizeof(float)), 0);

    // directional light
    shader.SetUniform3f("dirLight.direction", -2.0f, -1.0f, -0.3f);
    shader.SetUniform3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.SetUniform3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.SetUniform3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // point light 1
    shader.SetUniform3f("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    
    shader.SetUniform1f("pointLights[0].constant", lightAttenuation[0]);
    shader.SetUniform1f("pointLights[0].linear", lightAttenuation[1]);
    shader.SetUniform1f("pointLights[0].quadratic", lightAttenuation[2]);

    shader.SetUniform3f("pointLights[0].ambient", 0.19225f, 0.19225f, 0.19225f);
    shader.SetUniform3f("pointLights[0].diffuse", 0.50754f, 0.50754f, 0.50754f);
    shader.SetUniform3f("pointLights[0].specular", 0.508273f, 0.508273f, 0.508273f);
    shader.SetUniform1f("pointLights[0].shininess", 64.0f);

    
    // Load texture
    GLuint textures[6];
    glGenTextures(6, textures);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    SDL_Surface *imgFace = IMG_Load("resources/awesomeface.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgFace->w, imgFace->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgFace->pixels);
    //glUniform1i(glGetUniformLocation(shader.shaderID, "texFace"), 0);
    shader.SetUniform1i("texFace", 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    SDL_Surface *imgContainer = IMG_Load("resources/container.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgContainer->w, imgContainer->h, 0, GL_RGB, GL_UNSIGNED_BYTE, imgContainer->pixels);
    //glUniform1i(glGetUniformLocation(shader.shaderID, "texContainer"), 1);
    shader.SetUniform1i("texContainer", 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[2]);

    SDL_Surface *imgContainer2 = IMG_Load("resources/container2.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgContainer2->w, imgContainer2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgContainer2->pixels);
    //glUniform1i(glGetUniformLocation(shader.shaderID, "texContainer"), 1);
    shader.SetUniform1i("material.diffuseMap", 2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textures[3]);

    SDL_Surface *imgContainer2_Specular = IMG_Load("resources/container2_specular.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgContainer2_Specular->w, imgContainer2_Specular->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgContainer2_Specular->pixels);
    //glUniform1i(glGetUniformLocation(shader.shaderID, "texContainer"), 1);
    shader.SetUniform1i("material.specularMap", 3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, textures[4]);

    SDL_Surface *matrix_emission = IMG_Load("resources/matrix.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, matrix_emission->w, matrix_emission->h, 0, GL_RGB, GL_UNSIGNED_BYTE, matrix_emission->pixels);
    //glUniform1i(glGetUniformLocation(shader.shaderID, "texContainer"), 1);
    shader.SetUniform1i("material.emissionMap", 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, textures[5]);

    SDL_Surface *flashlight_texture = IMG_Load("resources/flashlight.jpg");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, flashlight_texture->w, flashlight_texture->h, 0, GL_RGB, GL_UNSIGNED_BYTE, flashlight_texture->pixels);
    shader.SetUniform1i("flashlightTexture", 5);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(imgFace);
    SDL_FreeSurface(imgContainer);
    SDL_FreeSurface(imgContainer2);
    SDL_FreeSurface(imgContainer2_Specular);
    SDL_FreeSurface(matrix_emission);
    SDL_FreeSurface(flashlight_texture);
}

void doRender()
{
    shader.Bind();
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    projectionMatrix = glm::perspective(glm::radians(fov), (GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    
    glm::vec3 cameraCenter;
    cameraCenter = cameraFront + cameraPos;
    viewMatrix = glm::lookAt(cameraPos, cameraCenter, cameraUp);

    GLint modelLoc = glGetUniformLocation(shader.shaderID, "model");
    GLint viewLoc = glGetUniformLocation(shader.shaderID, "view");
    GLint projectionLoc = glGetUniformLocation(shader.shaderID, "projection");

    // glm::vec3 diffuseCol = lightCol * glm::vec3(0.5f);
    // glm::vec3 ambientCol = diffuseCol * glm::vec3(0.2f);

    // shader.SetUniform3fv("light.position", 1, glm::value_ptr(lightPos));
    // shader.SetUniform3fv("light.direction", 1, glm::value_ptr(lightDir));
    shader.SetUniform2f("viewPort", WINDOW_WIDTH, WINDOW_HEIGHT);
    shader.SetUniform3fv("light.position", 1, glm::value_ptr(cameraPos));
    shader.SetUniform3fv("light.direction", 1, glm::value_ptr(cameraFront));
    shader.SetUniform1f("light.cutOff", glm::cos(glm::radians(20.5f)));
    shader.SetUniform1f("light.outerCutOff", glm::cos(glm::radians(23.5f)));
    shader.SetUniform3fv("viewPosition", 1, glm::value_ptr(cameraPos));
    
    // shader.SetUniform3f("dirLight.direction", 3.0f, -3.0f, -3.0f);
    // shader.SetUniform3fv("dirLight.ambient", 1, glm::value_ptr(ambientCol));
    // shader.SetUniform3fv("dirLight.diffuse", 1, glm::value_ptr(diffuseCol));
    // shader.SetUniform3f("dirLight.specular", 1.0f, 1.0f, 1.0f);


    shader.SetUniform1f("time", SDL_GetTicks());

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    for (GLuint i = 0; i < 8; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
        modelMatrix = glm::rotate(modelMatrix, glm::radians((float)SDL_GetTicks()/60), glm::vec3(1.0f, 0.3f, 0.5f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    if (pointLightActive)
    {
        // Light Object Rendering
        lightShader.Bind();

        modelLoc = glGetUniformLocation(lightShader.shaderID, "model");
        viewLoc = glGetUniformLocation(lightShader.shaderID, "view");
        projectionLoc = glGetUniformLocation(lightShader.shaderID, "projection");
        
        lightShader.SetUniform3fv("lightVal", 1, glm::value_ptr(lightCol));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        modelMatrix = glm::mat4(1.0f);
        //rotate light around origin using MATHS!!!
        if(isOrbitLight)
        {
            lightPos.x = sin(SDL_GetTicks()/1000.0f*2.05f) * 2.0f;
            lightPos.y = cos(SDL_GetTicks()/1000.0f*2.05f);
            lightPos.z = cos(SDL_GetTicks()/1000.0f*2.05f);
        }
        modelMatrix = glm::translate(modelMatrix, lightPos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    } 
}

void cleanRenderObjects()
{
    glDeleteProgram(shader.shaderID);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}