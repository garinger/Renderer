#version 460 core

// World coordinates
in vec4 frag_pos;

out vec4 fragment_color;

void main()
{
    float dist_from_origin = length(frag_pos);
    
    float fade = 1.0;
    if (dist_from_origin != 0.0)
    {
        fade = 1.0 / dist_from_origin;
    }

    vec3 color = vec3(0.5, 0.5, 0.5);

    fragment_color = vec4(color * fade, 1.0);
}