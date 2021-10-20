// Clase generica primitiva
#include <GL/glut.h>

class primitiva {
	char *nome;
};

class esfera:primitiva {
	int centerx, centery, radio;
};

class cono:primitiva {
	int centerx, centery, radio, altura;

public:
	void dibujar();
};