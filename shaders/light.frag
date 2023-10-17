#version 460 core

// This is the color that the light is emitting onto other objects in the scene
uniform vec3 color;

out vec4 fragment_color;

void main()
{
    fragment_color = vec4(color, 1.0);
}