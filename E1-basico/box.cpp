/////////////////////////////////          
// box.cpp
//
// This program draws a wire box.
//
// Sumanta Guha.
/////////////////////////////////

#include <iostream>
#include <GL/glut.h>

//#include "primitiva.h"

using namespace std;

//cono co;

// Drawing routine.
void drawScene(void) {
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 

   //co.dibujar();
   
   // Modeling transformations.
   glTranslatef(4.0, 0.0, -4.0);

   glutWireCube(5.0); // Box.
   glutSolidSphere(4.0,100.0,10.0);

   glFlush();
}

// Initialization routine.
void setup(void) {
   glClearColor(1.0, 1.0, 1.0, 0.0);  
}

// OpenGL window reshape routine.
void resize(int w, int h) {
	h = w = w < h ? w : h;
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 8.0, -5.0, 5.0, 5.0, 100.0);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
   switch(key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("box.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}

