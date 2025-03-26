#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D diffuseTexture;  // texture sampler
uniform vec3 lightDir;
uniform vec3 baseColor;

void main()
{
    float intensity = max(dot(normalize(Normal), normalize(-lightDir)), 0.0);

    // Basic "stepped" cel shading
    if (intensity > 0.95)
    intensity = 1.0;
    else if (intensity > 0.5)
    intensity = 0.7;
    else if (intensity > 0.25)
    intensity = 0.4;
    else
    intensity = 0.2;

    // Sample the texture color
    vec3 texColor = texture(diffuseTexture, TexCoords).rgb;
    // Combine with the cel shading intensity
    vec3 finalColor = texColor * intensity;

    FragColor = vec4(finalColor, 1.0);
}
