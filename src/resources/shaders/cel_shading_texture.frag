#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D diffuseTexture;// texture sampler
uniform vec3 lightDir;
uniform vec3 baseColor;
uniform bool uUseTexture;

void main()
{
    // compute cel‐shade intensity
    float intensity = max(dot(normalize(Normal), normalize(-lightDir)), 0.0);

    if (intensity > 0.95)
    intensity = 1.0;
    else if (intensity > 0.5)
    intensity = 0.7;
    else if (intensity > 0.25)
    intensity = 0.4;
    else
    intensity = 0.2;

    // choose texture or flat color
    vec3 color = uUseTexture
    ? texture(diffuseTexture, TexCoords).rgb
    : baseColor;

    FragColor = vec4(color * intensity, 1.0);
}
