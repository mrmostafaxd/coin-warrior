#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;


out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

 // Model matrix (transforms model vertices from model space to world space)
uniform mat4 M;
// Model inverse transpose matrix (transforms normals from model space to world space)
uniform mat4 M_IT;

// View-projection matrix (transforms world vertices from world space to clip space)
uniform mat4 VP;

// Eye position (in world space)
uniform vec3 eye;

void main(){
    // Transform vertex position from model space to world space
    vec3 world = (M * vec4(position, 1.0)).xyz;

    // Transform position from world space to clip space
    gl_Position = VP * vec4(world, 1.0);

    // Transform normal from model space to world space
    vs_out.normal = (M_IT * vec4(normal, 0.0)).xyz;

    // Compute view vector (in world space)
    vs_out.view = eye - world;

    // Pass color and texture coordinates to fragment shader
    vs_out.world = world;

    // Pass color and texture coordinates to fragment shader
    vs_out.color = color;

    // Pass color and texture coordinates to fragment shader
    vs_out.tex_coord = tex_coord;
}