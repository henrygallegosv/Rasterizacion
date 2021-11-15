#version 330 core

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
};

#define NR_POINT_LIGHTS 2

uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec4 normal;
in vec4 posicion;
//in vec3 colorFinal;
in vec2 v2f_texcoord;

uniform sampler2D textura0;

out vec4 FragColor;

vec3 CalcPointLight(PointLight ligth, vec3 normal, vec3 fragPos);

void main() {
   //vec4 colorLuz = vec4(1., 1., 1., 1.);
   //vec4 posicionLuz = vec4(1., 1., 10., 1.0);

   //vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
   //vec4 color = vec4(1.);
   //float kd = 0.8;

    // Compute the diffuse term.
    vec3 N = normalize( normal.xyz );
    //vec4 L = normalize( posicionLuz - posicion );
    //float NdotL = max( dot( N, L ), 0 );
    //vec4 Diffuse =  NdotL * colorLuz * color;
    //FragColor = Diffuse * texture( textura0, v2f_texcoord );

    vec3 result = vec3(0.0);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], N, posicion.xyz);

    FragColor = vec4(result, 1.0);

    // Especular
    // camara
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = light.ambient * vec3(texture(textura0, v2f_texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(textura0, v2f_texcoord));
    return (ambient + diffuse);

}