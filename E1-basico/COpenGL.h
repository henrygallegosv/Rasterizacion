#ifndef _CLASS_OPENGL_H_
#define _CLASS_OPENGL_H_

#include <GL/glut.h>
#include "primitiva.h"

class COpenGL {
	

public:
	cono co;
	//static float angulox;

	static void display(void);
	static void reshape(int width, int height);
	static void keyboard(unsigned char key, int x, int y);

	COpenGL(void);
	void Ejecutar(int argc, char **argv);
	// Initialization routine.
	void Setup(void);
};



#endif // _CLASS_OPENGL_H_