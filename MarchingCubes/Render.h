//#define cimg_use_png

#include "glut_ply.h"
#include "DistanceField.h"

#include <queue>
#include <gl/glew.h>
#include <gl/freeglut.h>

#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

#ifndef _RENDER_H_
#define _RENDER_H_

#include "Camera.h"

// #define M_PI			3.141592653
#define DEF_dollyStepSize	1.0

GLint leftMouseButton, rightMouseButton;    // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse

int drawMode = 0;
bool drawBoxes = true;

class Render{
public:
	Model_PLY model1;
	//Model_PLY model2;

	DistanceField	*df;
	static	Camera	cam;
	static	bool	bIsoSurface, bModel1, bDistanceField;
	static	bool	bOpWaveAbove;

	Render() {
		df = NULL;
	}

	void init_default() {
	}
	
	static void		display(void);
	static void		reshape(int width, int height);
	static void		keyboard(unsigned char key, int x, int y);
	static void		mouseCallback(int button, int state, int thisX, int thisY);
	static void		mouseMotion(int x, int y);
	static void		mouseWheel(int button, int dir, int x, int y);
	void	Ejecutar(int argc, char** argv);
	
	void	DrawAxis();
	void	DrawRed();
	void    DrawModel();
	void	DrawIsoSurface();

	void    Initialize();

}OBJ_RENDER;
Camera	Render::cam;
bool	Render::bIsoSurface = true;
bool	Render::bDistanceField = true;
bool	Render::bModel1 = false;
bool	Render::bOpWaveAbove = false;

void Render::Ejecutar(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Distance Field");

    //give the camera a scenic starting point.
	cam.setPosition(60, 40, 30);
	cam.setDirectionSpherical( -(float)M_PI / 3.0f, (float)M_PI / 2.8f );

    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutMouseFunc(Render::mouseCallback);
    glutMotionFunc(Render::mouseMotion);
    glutMouseWheelFunc(Render::mouseWheel);

	glutDisplayFunc(Render::display);
	glutReshapeFunc(Render::reshape);
	glutKeyboardFunc(Render::keyboard);

	Render::Initialize();

	glutMainLoop();
}

void Render::DrawAxis() {
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);
		
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);
	glEnd();	
}

void Render::display(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//clear the render buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);                //make sure we aren't changing the projection matrix!
    glLoadIdentity();
    gluLookAt(	cam.pos.x, cam.pos.y, cam.pos.z,			// camera is located at (x,y,z)
				cam.pos.x + cam.dir.x, cam.pos.y + cam.dir.y, cam.pos.z + cam.dir.z,	// camera is looking at at (x,y,z) + (dx,dy,dz) -- straight ahead
				cam.up.x, cam.up.y, cam.up.z);				// up vector is (0,1,0) (positive Y)
	
    glPushMatrix();
		OBJ_RENDER.DrawAxis();
		//if (drawBoxes)
		if (OBJ_RENDER.bDistanceField)
			OBJ_RENDER.DrawRed();
		if (OBJ_RENDER.bModel1)
			OBJ_RENDER.DrawModel();
		if (OBJ_RENDER.bIsoSurface)
			OBJ_RENDER.DrawIsoSurface();
	glPopMatrix();

	glutSwapBuffers();
}

void Render::reshape(int width, int height) {
	/*
	if (width <= height) {
		glViewport(0, 0, width, width);
	} else {
		glViewport(0, 0, height, height);
	}*/
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.0, 128.0);
	// gluPerspective(60.0, 1.0, 1.0, 128.0);
	cam.ratio = width * 1.0f / height;
	gluPerspective(cam.angle, cam.ratio, cam.nearP, cam.farP);
	cam.updateCameraInternals();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void Render::keyboard(unsigned char key, int x, int y) {
    if (key == 'h' || key == 'H') {
        printf("help\n\n");
        printf("c           - Toggle culling\n");
        printf("q/escape    - Quit\n\n");
    }

    if (key == 'c' || key == 'C') {
		glIsEnabled(GL_CULL_FACE) ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
    }

    if (key == 'q' || key == 'Q') {
        exit(0);
	}

	else if (key == 'a' || key == 'A') {
		//move side-left!
		cam.MoveSide(-2.0f);
    } 
	else if (key == 'd' || key == 'D') {
		// move side-right!
		cam.MoveSide(2.0f);
    } 
    else if (key == 'w' || key == 'W') {
		// move forward!
		// that's as simple as just moving along the direction.
		cam.pos += cam.dir * cam.movementConstant;
		cam.accelerating = true;
		cam.increaseCameraVelocity();
    } 
    else if (key == 's' || key == 'S') {
		// move backwards!
		// just move BACKWARDS along the direction.
		cam.pos -= cam.dir * cam.movementConstant;
    }

    else if (key == '+') {
        cam.increaseCameraVelocity();
    }

    else if (key == '-') {
        cam.decreaseCameraVelocity();
    }

    else if (key == 'P' || key == 'p') {
        drawMode = ++drawMode % 2;
        if (drawMode == 0) {                           // fill mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
    }

    else if (key == 'o' || key == 'O') {
        drawBoxes = !drawBoxes;
    }

	switch (key) {
	case 'i': case 'I': OBJ_RENDER.df->OperationWaveAbove(); break;
	case 'u': case 'U': OBJ_RENDER.df->operationOn	= false; OBJ_RENDER.df->getIsoSurface(); break;
	case '1': OBJ_RENDER.df->OperationCooking(); break;
	case '2': OBJ_RENDER.df->OperationElastic(); break;
	case '3': OBJ_RENDER.df->OperationErosion(); break;
	case '4': OBJ_RENDER.df->OperationDilation(); break;

	case '+': OBJ_RENDER.df->isoValue += 0.01f;  OBJ_RENDER.df->getIsoSurface(); break;
	case '-': OBJ_RENDER.df->isoValue -= 0.01f;  OBJ_RENDER.df->getIsoSurface(); break;

	case 'm': case 'M': OBJ_RENDER.bModel1			= !OBJ_RENDER.bModel1; break;
	case 'n': case 'N': OBJ_RENDER.bDistanceField	= !OBJ_RENDER.bDistanceField; break;
	case 'j': case 'J': OBJ_RENDER.bIsoSurface		= !OBJ_RENDER.bIsoSurface; break;
	}

	glutPostRedisplay();
}

/**
 * Save the state of the mouse button
 * when this is called so that we can check the status of the mouse
 * buttons inside the motion callback (whether they are up or down).
 */
void Render::mouseCallback(int button, int state, int thisX, int thisY) {
    //update the left and right mouse button states, if applicable
    if(button == GLUT_LEFT_BUTTON)
        leftMouseButton = state;
    else if(button == GLUT_RIGHT_BUTTON)
        rightMouseButton = state;

    //and update the last seen X and Y coordinates of the mouse
    mouseX = thisX;
    mouseY = thisY;
}

void Render::mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) {
		cam.increaseCameraVelocity();
    } else {
		cam.decreaseCameraVelocity();
    }
    return;
}

/**
 * update cameraPhi, cameraTheta, and/or
 * cameraRadius based on how much the user has moved the mouse in the
 * X or Y directions (in screen space) and whether they have held down
 * the left or right mouse buttons. If the user hasn't held down any
 * buttons, the function just updates the last seen mouse X and Y coords.
 */
void Render::mouseMotion(int x, int y) {
    if (leftMouseButton == GLUT_DOWN) {
		cam.theta	+= (x - mouseX) * 0.005f;
		cam.phi		+= (mouseY - y) * 0.005f;

		 // make sure that phi stays within the range (0, M_PI)
		if (cam.phi <= 0)
			cam.phi = 0+0.001f;
		if (cam.phi >= (float)M_PI)
			cam.phi = (float)M_PI-0.001f;

		cam.recomputeOrientation();	//update camera (x,y,z) based on (radius,theta,phi)
		glutPostRedisplay();
    }
	
    mouseX = x;
    mouseY = y;
}

void Render::DrawRed() {
	glColor3f(0.0f, 1.0f, 1.0f);
	// df->getOpenGL();
	df->getPointsOpenGL();
}

void Render::DrawModel() {
    glColor3f(.60f, .60f, .60f);
    model1.Draw();
}

void Render::DrawIsoSurface() {
	glColor3f(0.2f, 0.2f, 0.8f);
	df->getIsoSurfaceOpenGL();
}

void Render::Initialize () {
   // select projection matrix
    glMatrixMode(GL_PROJECTION);

    // set matrix mode
    glMatrixMode(GL_PROJECTION);

    // reset projection matrix
    glLoadIdentity();

    // specify which matrix is the current matrix
    glMatrixMode(GL_MODELVIEW);
    glShadeModel( GL_SMOOTH );

    // specify the clear value for the depth buffer
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    // specify implementation-specific hints
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	const GLfloat luzPos[]	= { 20.0, 20.0, 20.0, 0.0 };
    const GLfloat ambient[]	= { 0.50, 0.50, 0.50, 1.0 };
    const GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    const GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // enable a light
	glLightfv(GL_LIGHT1, GL_POSITION, luzPos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);

    // enable depth-testing, colored materials, and lighting
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // normalize normals so lighting calculations are correct
    // when using GLUT primitives
    glEnable(GL_NORMALIZE);

    // enable smooth shading
    glShadeModel(GL_SMOOTH);

    glClearColor(0.9f, 0.9f, 1.0f, 0.0f);
}

#endif
