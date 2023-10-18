#version 460 core

// https://en.wikipedia.org/wiki/Phong_reflection_model

#define MAX_LIGHTS 1

in vec3 position;
in vec3 normal;
in vec2 texture_coords;

uniform sampler2D model_texture;

struct light
{
    vec3 position;
    vec3 specular;
    vec3 diffuse;
    vec3 ambient;
};

struct material
{
    float specular;
    float diffuse;
    float ambient;
    float shininess;
};

uniform light lights[MAX_LIGHTS];
uniform material mat;

uniform vec3 camera_look;

out vec4 fragment_color;

void main()
{
    vec3 final_color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        // Add the ambient lighting from the i'th light
        final_color += mat.ambient * lights[i].ambient;

        vec3 norm = normalize(normal);
        vec3 frag_to_light = normalize(lights[i].position - position);
        float face_dot = dot(frag_to_light, norm);

        // Specular
        vec3 frag_to_look = normalize(camera_look - position);
        vec3 r = 2 * dot(frag_to_light, norm) * norm - frag_to_light;
        float r_dot_camera = dot(r, frag_to_look);

        vec3 light_contribution = vec3(0.0, 0.0, 0.0);

        if (face_dot > 0.0)
        {
            light_contribution = (mat.diffuse * face_dot * lights[i].diffuse);
            
            if (r_dot_camera > 0.0)
            {
                light_contribution += (mat.specular * pow(r_dot_camera, mat.shininess) * lights[i].specular);
            }
        }

        // Attenuation
        float kc = 1.0;
        float kl = 0.09;
        float kq = 0.032;
        float dist_to_light = length(lights[i].position - position);
        float attenuation = 1.0 / (kc + kl * dist_to_light + kq * (dist_to_light * dist_to_light));

        final_color += light_contribution * attenuation;
    }
    
    fragment_color = vec4(final_color, 1.0);
}











