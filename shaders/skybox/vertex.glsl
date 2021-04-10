#version 330

layout(location=0) in vec3 a_position;

uniform mat4 u_projection;
uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_normal;

out vec3 position;
out vec3 normal;

void main() {
    position = a_position;
    
    gl_Position = u_projection * u_view * u_world * vec4(a_position, 1.0);
}