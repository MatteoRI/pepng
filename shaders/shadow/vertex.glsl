#version 300 es

precision highp float;

layout (location = 0) in vec3 a_position;

uniform mat4 u_world;

out vec4 FragPos;

void main() {
    FragPos = u_world * vec4(a_position, 1.0);
    gl_Position = FragPos;
}