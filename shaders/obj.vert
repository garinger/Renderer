#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec3 position;
out vec3 normal;
out vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	position = vec3(model * vec4(pos, 1.0));
	normal = norm;
	texture_coords = tex;
	gl_Position = projection * view * model * vec4(pos, 1.0);
}