#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;// (unused if we’re just doing toon shading)

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 Normal;
out vec2 TexCoords; // (optional)

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    // If your model or view might scale/rotate normals, you might do
    // mat3 normalMatrix = transpose(inverse(mat3(uModel * uView)));
    // Normal = normalize(normalMatrix * aNormal);

    Normal = aNormal;
    TexCoords = aTexCoords;
}
