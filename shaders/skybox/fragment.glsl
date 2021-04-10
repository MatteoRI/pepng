#version 330

#define PI 3.1415926535897932384626433832795

uniform sampler2D u_texture;

uniform bool u_display_texture;

in vec3 position;

out vec4 color;

void main() {
    // Project cube position onto sphere.
    vec3 direction = normalize(position);

    // Convert to spherical coordiantes.
    float delta = atan(direction.y, direction.x);
    float phi = acos(direction.z / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z));

    // Scale spherical coordiantes to UV. 
    vec2 uv = vec2(delta / (2 * PI) + 1, phi / PI);

    // Sample from HDR.
    color = texture(u_texture, uv);
}