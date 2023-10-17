#version 460 core

#define MAX_LIGHTS 2

in vec3 position;
in vec3 normal;
in vec2 texture_coords;

uniform sampler2D model_texture;

struct light
{
 vec3 position;
 vec3 color;
};

uniform light lights[MAX_LIGHTS];

out vec4 fragment_color;

void main()
{
    vec3 final_color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        vec3 norm = normalize(normal);
        vec3 frag_to_light = normalize(lights[i].position - position);
        float face_dot = dot(frag_to_light, norm);

        if (face_dot > 0.0)
        {
            final_color += lights[i].color * face_dot;
        }
    }

    fragment_color = vec4(final_color, 1.0);
}