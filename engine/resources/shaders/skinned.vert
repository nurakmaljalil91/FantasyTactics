#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in ivec4 aBoneIds;
layout(location = 4) in vec4 aWeights;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out float vHasWeights;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

const int MAX_BONES = 128;
uniform mat4 uBones[MAX_BONES];
uniform bool uUseSkinning;

void main()
{
    mat4 skinMatrix = mat4(1.0);
    vHasWeights = 0.0;
    if (uUseSkinning) {
        ivec4 boneIds = clamp(aBoneIds, 0, MAX_BONES - 1);
        skinMatrix = mat4(0.0);
        skinMatrix += uBones[boneIds[0]] * aWeights[0];
        skinMatrix += uBones[boneIds[1]] * aWeights[1];
        skinMatrix += uBones[boneIds[2]] * aWeights[2];
        skinMatrix += uBones[boneIds[3]] * aWeights[3];

        if (aWeights.x + aWeights.y + aWeights.z + aWeights.w == 0.0) {
            skinMatrix = mat4(1.0);
        } else {
            vHasWeights = 1.0;
        }
    }

    vec4 skinnedPosition = skinMatrix * vec4(aPos, 1.0);
    vec3 skinnedNormal = mat3(skinMatrix) * aNormal;

    FragPos = vec3(uModel * skinnedPosition);
    Normal = mat3(transpose(inverse(uModel))) * skinnedNormal;
    TexCoords = aTexCoord;

    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}
