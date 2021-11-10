#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

using namespace std;

// Function to read text file.
char* readTextFile(char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");	
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char)); 
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

void Error(char *message) {
    printf(message);
}

/* Compila shader */
void CompileShader (GLuint id) {
    GLint status;
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*) malloc(len*sizeof(char));
        glGetShaderInfoLog(id, len, 0, message);
        Error(message);
        free(message);
    }
}


// Function to initialize shaders.
int setShader(char* shaderType, char* shaderFile)
{
   int shaderId;
   char* shader = readTextFile(shaderFile);
   
   if (shaderType == "vertex") shaderId = glCreateShader(GL_VERTEX_SHADER); 
   if (shaderType == "tessControl") shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);    
   if (shaderType == "tessEvaluation") shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER); 
   if (shaderType == "geometry") shaderId = glCreateShader(GL_GEOMETRY_SHADER); 
   if (shaderType == "fragment") shaderId = glCreateShader(GL_FRAGMENT_SHADER); 

   glShaderSource(shaderId, 1, (const char**) &shader, NULL); 
   //glCompileShader(shaderId);
   CompileShader(shaderId);

   return shaderId;
}

