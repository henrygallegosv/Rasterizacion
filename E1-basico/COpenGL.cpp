#include "COpenGL.h"

COpenGL OBJ_OGL;

COpenGL::COpenGL(){
}

void COpenGL::Ejecutar(int argc, char **argv) {
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Comp. Grafica");
	Setup();

	glutDisplayFunc(COpenGL::display);
	glutReshapeFunc(COpenGL::reshape);
	glutKeyboardFunc(COpenGL::keyboard);

	glutMainLoop();
}

// Initialization routine.
void COpenGL::Setup(void) {
	// Cor de fondo.
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//angulox = 0;
}

void COpenGL::display(void) {
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 
   
   // Modeling transformations.
   glTranslatef(0.0, 0.0, -8.0);

   //glRotatef(angulox, 1., 0., 0.);

   glutWireCube(5.0); // Box.
   //glutSolidSphere(4.0,100.0,10.0);
   OBJ_OGL.co.dibujar();

   glFlush();
}

void COpenGL::reshape(int width, int height) {
	height = width = width < height ? width : height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
}

void COpenGL::keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case '2':
		//angulox += 1.0;
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}