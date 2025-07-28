// text.vert
// Vertex shader for rendering text using texture mapping
// This shader takes vertex positions and texture coordinates as input
// and outputs the transformed position and texture coordinates for the fragment shader.
#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
