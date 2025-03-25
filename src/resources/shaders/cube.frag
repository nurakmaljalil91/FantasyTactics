#version 330 core

in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
