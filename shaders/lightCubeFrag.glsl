#version 330 core
out vec4 FragColor;
uniform vec3 lightVal;

void main()
{
    FragColor = vec4(lightVal, 1.0f);
}