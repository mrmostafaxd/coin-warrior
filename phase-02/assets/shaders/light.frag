#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
//sampler for each type
struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emission;
};

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light {
    int type;
    vec3 position; //from this entity M
    vec3 direction;//from this entity M
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
};

struct SkyLight {
    vec3 sky, horizon, ground;
};
// for Deffuse
float lambert(vec3 n, vec3 l){
    // depends on the angle between normal and light direction on the object
    return max(0, dot(n, l));
}
// for specular
float phong(vec3 n, vec3 l, vec3 v, float shininess){
    vec3 r = reflect(-l, n);
    //depends on the angle between reflect and view direction
    return pow(max(0, dot(v, r)), shininess);
}

vec3 compute_sky_light(vec3 normal, SkyLight sky_light) {
    // Calculate sky light factor based on normal y component
    float y = normal.y;
    
    // Square sky factor for smoother transition
    float sky_factor = max(0, y);
    
    // Calculate ground light factor based on negative of normal y component
    float ground_factor = max(0, -y);
    sky_factor *= sky_factor;

    // Square ground factor for smoother transition
    ground_factor *= ground_factor;

     // Calculate horizon light factor as 1 minus sky and ground factors
    float horizon_factor = 1 - sky_factor - ground_factor;

    // Return weighted sum of sky, horizon, and ground light colors
    return sky_light.sky * sky_factor + sky_light.horizon * horizon_factor + sky_light.ground * ground_factor;
}

#define MAX_LIGHTS 8

uniform Material material;
uniform int light_count;
uniform Light lights[MAX_LIGHTS];
uniform SkyLight sky_light;

void main(){
    //normal on point
    vec3 normal = normalize(fs_in.normal);
    //from camera to POINT
    vec3 view = normalize(fs_in.view);

    //we read from all texures simultanously as we created different texure units
    vec3 material_albedo = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 material_specular = texture(material.specular, fs_in.tex_coord).rgb;
    //take only r channel as it is grayscale
    float roughness = texture(material.roughness, fs_in.tex_coord).r;
    float shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
    vec3 material_emission = texture(material.emission, fs_in.tex_coord).rgb;
    float material_ao = texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    //material constants before applying lights
    frag_color = vec4(material_emission + material_albedo * material_ao * compute_sky_light(normal, sky_light), 1);

    for(int i = 0; i < min(MAX_LIGHTS, light_count); i++){
        Light light = lights[i];

        // becquse lambert assumes that light is coming from the point to the light source
        vec3 light_vec = - light.direction;

        // because directional light has no position and parallel in all constant direction
        if(light.type != DIRECTIONAL){
            // need to know the direction from the point to the light source for point light and spot light

            /*
                light_vec is a variable representing the direction from the surface being shaded to the light source. This direction is calculated by subtracting the world space position of the surface from the position of the light source and then normalizing the resulting vector.

                The normalize function scales the vector such that its length is equal to 1, making it a unit vector. This is often useful when working with lighting calculations because it allows the direction of the vector to be used without being affected by the distance between the surface and the light source.
            */
            light_vec = normalize(light.position - fs_in.world);
        }

        vec3 diffuse = material_albedo * light.color * lambert(normal, light_vec);
        vec3 specular = material_specular * light.color * phong(normal, light_vec, view, shininess);

        float attenuation = 1;
        // because directional light has constant intensity
        if(light.type != DIRECTIONAL){
            float d = distance(light.position, fs_in.world);
            attenuation *= 1.0/dot(light.attenuation, vec3(1, d, d*d));
            if(light.type == SPOT){
                //light.direction in the middle of the inner cone
                //outer cone - inner cone
                float angle = acos(-dot(light.direction, light_vec));
                //inside inner cone will be  full intensity 
                //between inner and outer cone will  gradually decrease intensity 
                //outside outer cone will be no intensity 
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        frag_color.rgb += (diffuse + specular) * attenuation;
    }
}