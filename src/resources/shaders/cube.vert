#version 330 core
layout (location = 0) in vec3 aPos;      // matches position in your Vertex struct
layout (location = 1) in vec3 aNormal;   // matches normal in your Vertex struct
layout (location = 2) in vec2 aTexCoords;// matches texCoords in your Vertex struct

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 Normal;
out vec2 TexCoords;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    Normal = aNormal;
    TexCoords = aTexCoords;
}
