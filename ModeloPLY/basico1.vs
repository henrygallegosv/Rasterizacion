#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//out vec3 normal;
//out vec3 posicion;
out vec3 colorFinal;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   //posicion = aPos;
   //normal = aNormal;

   vec3 posicionLuz = vec3(1., 1., -10.);
   vec3 colorLuz = vec3(1., 1., 1.);
   vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
   float kd = 0.8;
   float diff = max(0.0, dot(normalize(posicionLuz - aPos), aNormal));
   colorFinal = color.rgb * colorLuz * kd * diff;
   //colorFinal = clamp(colorFinal);
}