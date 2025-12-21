#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec4 uTint;

void main()
{
    vec4 tex = texture(uTexture, TexCoord);
    FragColor = tex * uTint;
}
