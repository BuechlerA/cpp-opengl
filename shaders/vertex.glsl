#version 330 core
in vec3 pos;
in vec3 normal;
in vec3 color;
uniform vec3 lightColor;
in vec2 texcoord;
out vec3 Color;
out vec3 LightColor;
out vec2 Texcoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    Color = color;
    LightColor = lightColor;
    Texcoord = texcoord;
    //Normal = normal;
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(pos, 1.0f));
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0f);
}