#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in float vHasWeights;

out vec4 FragColor;

uniform sampler2D diffuseTexture;  // your 2D texture
uniform bool        uUseTexture;   // toggle: true = sample texture, false = use baseColor
uniform vec3        baseColor;     // fallback flat color
uniform vec3        lightDir;      // direction _to_ the light
uniform vec3        lightColor;    // light tint
uniform float       ambientStrength; // e.g. 0.2-0.3
uniform float       lightIntensity; // overall lighting boost
uniform float       lightWrap; // softens shadows by wrapping diffuse

void main()
{
    // 1. pick your color source
    vec3 color = uUseTexture
    ? texture(diffuseTexture, TexCoords).rgb
    : baseColor;

    if (vHasWeights > 0.5) {
        color = vec3(0.2, 0.9, 0.2);
    }

    // 2. normalize vectors
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDir);

    // 3. ambient term
    vec3 ambient = ambientStrength * lightColor * color;

    // 4. diffuse term (Lambert)
    float diff = max(dot(N, L), 0.0);
    float wrapped = clamp((diff + lightWrap) / (1.0 + lightWrap), 0.0, 1.0);
    vec3 diffuse = wrapped * lightColor * color;

    // 5. combine
    vec3 result = (ambient + diffuse) * lightIntensity;
    FragColor = vec4(result, 1.0);
}
