#version 460 core

layout (location = 0) in vec3 pos;

layout (std140, binding = 0) uniform camera
{
	mat4 view;
	mat4 projection;
	vec4 forward;
};

uniform mat4 model;

out vec4 frag_pos;

void main()
{
	frag_pos = model * vec4(pos, 1.0);
	gl_Position = projection * view * model * vec4(pos, 1.0);
}