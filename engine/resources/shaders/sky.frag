#version 330 core

in vec2 NdcPos;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform mat4 uInvViewRot;

void main()
{
    vec3 viewDir = normalize(vec3(NdcPos, 1.0));
    vec3 worldDir = normalize((uInvViewRot * vec4(viewDir, 0.0)).xyz);

    float u = 0.5 + atan(worldDir.z, worldDir.x) / (2.0 * 3.14159265359);
    float v = 0.5 - asin(worldDir.y) / 3.14159265359;

    FragColor = texture(diffuseTexture, vec2(u, v));
}
