#version 300 es

precision highp float;

uniform sampler2D u_texture;

uniform bool u_has_color;
uniform vec3 u_color;

in vec2 tex_coord;

out vec4 color;

void main() {
    if(u_has_color) {
        color = vec4(u_color, 1.0);
    } else {
        color = texture(u_texture, tex_coord);
    }
}