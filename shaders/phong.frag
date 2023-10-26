#version 460 core

// https://en.wikipedia.org/wiki/Phong_reflection_model

#define MAX_NUM_LIGHTS 32

struct Light
{
    vec4 position;
    vec4 specular;
    vec4 diffuse;
    vec4 ambient;
};

struct Material
{
    float specular;
    float diffuse;
    float ambient;
    float shininess;
};

in vec3 position;
in vec3 normal;

layout (std140, binding = 0) uniform camera
{
                        // Base alignment   Aligned offset
	mat4 view;          // 4 * 16           0
	mat4 projection;    // 4 * 16           64
	vec4 forward;       // 16               128
};

layout (std140, binding = 1) uniform lightSources
{
                                     // Base alignment              Aligned offset
    Light lights[MAX_NUM_LIGHTS];    // 4 * 16 * MAX_NUM_LIGHTS     0
    unsigned int num_active_lights;  // 4                           4 * 16 * MAX_NUM_LIGHTS
};

uniform Material material;

out vec4 fragment_color;

void main()
{
    vec3 final_color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < num_active_lights; i++)
    {
        // TODO: Refactor this?
        vec3 lightPos = lights[i].position.xyz;
        vec3 lightSpec = lights[i].specular.xyz;
        vec3 lightDiff = lights[i].diffuse.xyz;
        vec3 lightAmbi = lights[i].ambient.xyz;
        vec3 cameraForward = forward.xyz;

        // Add the ambient lighting from the i'th light
        final_color += material.ambient * lightAmbi;

        vec3 norm = normalize(normal);
        vec3 frag_to_light = normalize(lightPos - position);
        float face_dot = dot(frag_to_light, norm);

        // Specular
        vec3 frag_to_look = normalize(cameraForward - position);
        vec3 r = 2 * dot(frag_to_light, norm) * norm - frag_to_light;
        float r_dot_camera = dot(r, frag_to_look);

        vec3 light_contribution = vec3(0.0, 0.0, 0.0);

        if (face_dot > 0.0)
        {
            light_contribution = (material.diffuse * face_dot * lightDiff);
            
            if (r_dot_camera > 0.0)
            {
                light_contribution += (material.specular * pow(r_dot_camera, material.shininess) * lightSpec);
            }
        }

        // Attenuation
        // The light contribution from each light should diminish as it gets further away
        // from the object. 
        float kc = 1.0;
        float kl = 0.09;
        float kq = 0.032;
        float dist_to_light = length(lightPos - position);
        float attenuation = 1.0 / (kc + kl * dist_to_light + kq * (dist_to_light * dist_to_light));

        final_color += light_contribution * attenuation;
    }
    
    fragment_color = vec4(final_color, 1.0);
}