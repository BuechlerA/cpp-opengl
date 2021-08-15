#version 330 core
in vec3 Color;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFace;
uniform sampler2D texContainer;
void main()
{
    vec4 colFace = texture(texFace, Texcoord);
    vec4 colContainer = texture(texContainer, Texcoord);
    
    outColor = mix(colContainer, colFace, 0.5f) * vec4(Color, 1.0f);
    // outColor = vec4(Color, 1.0f);
    // outColor = vec4(1.0f);
}
