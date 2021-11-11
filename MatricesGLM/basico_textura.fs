#version 330 core

in vec4 normal;
in vec4 posicion;
//in vec3 colorFinal;
in vec2 v2f_texcoord;

uniform sampler2D textura0;

out vec4 FragColor;

void main() {
   vec3 colorLuz = vec3(1., 1., 1.);
   vec3 posicionLuz = vec3(1., 1., 10.);

   vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
   float kd = 0.8;

   float diff = max(0.0, dot(normalize(posicionLuz - posicion.xyz/posicion.w), normal.xyz/normal.w));
   //float diff = max(0.0, dot(normalize(posicionLuz - posicion.xyz), normal.xyz));
   FragColor = vec4(color.rgb * colorLuz * kd * diff, 1.0) * texture( textura0, v2f_texcoord );

   //FragColor = vec4(colorFinal, 1.0f);
   //FragColor = colorFinal;
}