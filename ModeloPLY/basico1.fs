#version 330 core

//in vec3 normal;
//in vec3 posicion;
in vec3 colorFinal;

out vec4 FragColor;

void main() {
   //vec3 colorLuz = vec3(1., 1., 1.);
   //vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
   //float kd = 0.8;
   //FragColor = color.rgb * colorLuz * kd * dot(posicion.xyz, normal);
   FragColor = vec4(colorFinal, 1.0f);
   //FragColor = colorFinal;
}