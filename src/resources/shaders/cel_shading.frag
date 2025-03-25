#version 330 core

in vec3 Normal;
out vec4 FragColor;

// Uniforms for a single directional light
uniform vec3 lightDir;
uniform vec3 baseColor;

void main()
{
    // Normalize everything
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightDir);

    // 1. Ambient factor
    float ambient = 0.3; // tweak to taste

    // 2. Diffuse factor (Lambert)
    //    Note: Depending on your convention, you may want dot(N, +L) or dot(N, -L).
    float diffuse = max(dot(N, -L), 0.0);

    // Combine them (but don’t exceed 1.0)
    float intensity = clamp(ambient + diffuse, 0.0, 1.0);

    // 3. Quantize the intensity to get discrete “toon” bands
    float levels = 4.0; // e.g. 4 or 5 levels
    float stepped = floor(intensity * levels) / levels;

    // 4. Final color
    FragColor = vec4(baseColor * stepped, 1.0);
}
