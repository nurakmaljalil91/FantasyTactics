#version 330 core

layout(location = 0) in vec3 aPos;

out vec2 NdcPos;

void main()
{
    NdcPos = aPos.xy * 2.0;
    gl_Position = vec4(NdcPos, 0.0, 1.0);
}
