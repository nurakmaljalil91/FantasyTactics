#version 330 core

in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D diffuseTexture;  // your 2D texture
uniform bool        uUseTexture;   // toggle: true = sample texture, false = use baseColor
uniform vec3        baseColor;     // fallback flat color
uniform vec3        lightDir;      // direction _to_ the light
uniform float       ambientStrength; // e.g. 0.2-0.3
uniform float       lightIntensity; // overall lighting boost

void main()
{
    // 1. pick your color source
    vec3 color = uUseTexture
    ? texture(diffuseTexture, TexCoords).rgb
    : baseColor;

    // 2. normalize vectors
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDir);

    // 3. ambient term
    vec3 ambient = ambientStrength * color;

    // 4. diffuse term (Lambert)
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * color;

    // 5. combine
    vec3 result = (ambient + diffuse) * lightIntensity;
    FragColor = vec4(result, 1.0);
}
