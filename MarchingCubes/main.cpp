#include <iostream>
using namespace std;

#include "Render.h"
#include "DistanceField.h"
#include "glut_ply.h"

int main( int argc, char **argv ){
	float		scale, extra;
	Model_PLY	model;
	DistanceField	df;
	
	int test = 2;
	switch (test) {
	case 0:
		scale = 10.0;
		model.Load((char*) "models/fighter.ply", scale);
		extra = 4.0f;
		df.setWidthVoxel( 0.5f );
		df.pos	= model.posMin - extra;
		df.tam	= ((model.posMax - model.posMin) + 2*extra) / df.width;
		break;
	case 1:
		scale = 90.0;
		model.Load((char*) "models/bunny.ply", scale);
		extra = 4.0f;
		df.setWidthVoxel( 0.2f );
		df.pos	= model.posMin - extra;
		df.tam	= ((model.posMax - model.posMin) + 2*extra) / df.width;
		break;
	case 2:
		scale = 80.0;
		model.Load((char*) "models/bunny.ply", scale);
		extra = 4.0f;
		df.setWidthVoxel( 0.6f );
		df.pos	= model.posMin - extra;
		df.tam	= ((model.posMax - model.posMin) + 2*extra) / df.width;
		break;
	case 3:
		scale = 10.0;
		model.Load((char*) "models/cow.ply", scale);
		extra = 4.0f;
		df.setWidthVoxel( 0.5f );
		df.pos	= model.posMin - extra;
		df.tam	= ((model.posMax - model.posMin) + 2*extra) / df.width;
		break;
	case 4:
		scale = 10.0;
		model.Load((char*) "models/horse.ply", scale);
		extra = 2.0f;
		df.setWidthVoxel( 0.5f );
		df.isoValue = 0.40f;
		df.pos	= model.posMin - extra;
		df.tam	= ((model.posMax - model.posMin) + 2*extra) / df.width;
		break;
	case 5:
		scale = 30.0;
		model.Load((char*) "models/rhino.ply", scale);
		extra = 2.0f;
		df.setWidthVoxel( 0.4f );
		df.pos	= model.posMin - extra;
		df.tam	= ((model.posMax - model.posMin) + 2*extra) / df.width;
		break;
	}
	
	df.triangles = model.triangles;
	df.Process();
	//df.Print();

	OBJ_RENDER.model1	= model;
	OBJ_RENDER.df		= &df;
	OBJ_RENDER.Ejecutar(argc, argv);

	cin.get();
}