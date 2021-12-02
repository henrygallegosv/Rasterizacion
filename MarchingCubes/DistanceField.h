
#include <iostream>
using namespace std;

#ifndef _DISTANCE_FIELD_H_
#define _DISTANCE_FIELD_H_

#include <glew.h>
#include <freeglut/freeglut.h>

#include <vector>
#include "Voxel.h"
#include "mutable_priority_queue.h"
#include "vmath.h"
#include "MarchinCubes.h"

class DistanceField {
public:
	Voxel 		***box, ***boxCopy;
	Vector3f	pos;	// position of Voxel (vertex minor)
	float		width;	// width of a voxel (cubo)
	float		width2, distWidth2;
	Vector3i	tam;	// cantidad de voxels en cada dimension
	mutable_priority_queue<float,Voxel*,std::less<float>> heap;
	vector<Voxel*>		voxelInitials;

	vector<Triangle*>	*triangles, *triangIsoSurface, *triangIsoSurfaceTmp;
	Vector3f	posInfTri, posSupTri;

	float		distMax;
	bool		operationOn;
	int			nivel;
	float		isoValue;	// For extract to isosurface

	DistanceField() {
		triangles = NULL;
		triangIsoSurface = triangIsoSurfaceTmp = NULL;
		box = boxCopy = NULL;
		operationOn = false;
	}

	void	setWidthVoxel(float tWidth) {
		width	= tWidth;
		width2	= width / 2;
		distWidth2 = width2 * sqrt(3.0f);
		isoValue = distWidth2;
	}

	void	Process();
	void	Init();
	void	Initialization(vector<Voxel*> &vVoxel);
	void	Loop();
	void	Print();
	void	getOpenGL();
	void	getPointsOpenGL();
	void	getVoxelsOpenGL();

	void	CopyBox();
	
	/* Con Marching Cubes */
	void	getIsoSurface(bool copy=false);
	void	getIsoSurfaceOpenGL();

	/** Voxelization initial **/
	void	VoxelizeTriangles();

	/* OPERATIONS */
	void	OperationWaveAbove();
	void	IncreaseFromAbove(int canNivel, float canIncrease);
	void	OperationCooking();
	void	OperationElastic();
	void	OperationErosion();
	void	OperationDilation();

private:
	// Six diferent steps
	Vector3i	idxN6[6];
	bool		HIGH_ACCURACY;
	static	Vector3f	colorTable[16];

	/** Voxelization initial **/
	Vector3i	getIdxVoxelOfPoint(Vector3f &p);
	float		getDistancePointTriangle(Vector3f &p, Triangle *&tri, Voxel &tmpVxl);
	float		getDistancePointLine(Vector3f &p, Vector3f &p0, Vector3f &p1);

	/* FAST MARCHING METHODS */
	void	getNeighbour(Vector3i idx, int &rCan, Vector3i *&resIdxNei);
	bool	ComputeDistance(Vector3i idx, float &maxSol);
	
	// Is a function which returns the state of a voxel given a 3D integer vector.
	int		State(Vector3i idx) { 
		if (operationOn) {
			return boxCopy[idx.x][idx.y][idx.z].state;
		}
		return box[idx.x][idx.y][idx.z].state; 
	}
	int		State(int ix, int iy, int iz) { return box[ix][iy][iz].state; }
	void	setState(Vector3i idx, int stt) { box[idx.x][idx.y][idx.z].state = stt; }

	// Returns the distance value of the voxel.
	// If the state is FAR, this value is, of course, undefined.
	float	Value(Vector3i idx) {
		if (operationOn) {
			return boxCopy[idx.x][idx.y][idx.z].distance;
		}
		if (State(idx) == VOXEL_FAR)
			return FLT_MAX;
		return box[idx.x][idx.y][idx.z].distance;
	}
	float	Value(int ix, int iy, int iz) {
		if (State(ix, iy, iz) == VOXEL_FAR)
			return FLT_MAX;
		return box[ix][iy][iz].distance;
	}
	void	setValue(Vector3i idx, float val) {
		box[idx.x][idx.y][idx.z].distance = val;
	}
	void	setInner(Vector3i idx, bool inner) {
		box[idx.x][idx.y][idx.z].inner = inner;
	}

	// Get the pointer of a given index
	Voxel*	getPointerVoxel(Vector3i idx) {
		return &box[idx.x][idx.y][idx.z];
	}

	// Index is valid?
	bool	isIndexValid(Vector3i idx) {
		if (idx.x < 0 || idx.x >= tam.x) return false;
		if (idx.y < 0 || idx.y >= tam.y) return false;
		if (idx.z < 0 || idx.z >= tam.z) return false;
		return true;
	}

	// Compute the solution to second order polynomials.
	int		SolveQuadric(float coeff[3], float sol[2]);
};

void DistanceField::Init() {
	HIGH_ACCURACY = true;

	int ix, iy, iz;
	box = new Voxel**[tam.x];
	for (ix = 0; ix < tam.x; ix++) {
		box[ix] = new Voxel*[tam.y];
		for (iy = 0; iy < tam.y; iy++) {
			box[ix][iy] = new Voxel[tam.z];
			for (iz = 0; iz < tam.z; iz++) {
				box[ix][iy][iz].setIndex(ix, iy, iz);
			}
		}
	}
	// The neighbours of a voxel are those reachable by taking one step in any of
	// six directions parallel to the primary axes.
	idxN6[0] = Vector3i(-1,  0,  0);
	idxN6[1] = Vector3i( 1,  0,  0);
	idxN6[2] = Vector3i( 0, -1,  0);
	idxN6[3] = Vector3i( 0,  1,  0);
	idxN6[4] = Vector3i( 0,  0, -1);
	idxN6[5] = Vector3i( 0,  0,  1);
}

// vVoxel is a vector of pointers a Voxel 
void DistanceField::Initialization(vector<Voxel*> &vVoxel) {
	int		i, j;
	int		canNei;
	float	dist;
	Vector3i *idxNei;
	distMax = 0;

	// [For each voxel v]
	for (i = 0; i < (int)vVoxel.size(); i++) {
		// [Freeze v]
		vVoxel[i]->state = VOXEL_FROZEN;

		// [For each neighbour vn of v]  not Frozen
		getNeighbour(vVoxel[i]->index, canNei, idxNei);
		for (j = 0; j < canNei; j++) {
			if (State(idxNei[j]) == VOXEL_FROZEN)
				continue;

			// [Compute distance d at vn]
			if ( !ComputeDistance(idxNei[j], dist) )
				continue;

			// Update distMax
			if (dist > distMax) 
				distMax = dist;

			// Update only if the new value is less
			if ( dist >= Value(idxNei[j]) )
				continue;
			setValue(idxNei[j], dist);
			setInner(idxNei[j], vVoxel[i]->inner);

			// [if vn is not in narrow band]
			if (State(idxNei[j]) == VOXEL_FAR) {
				// [tag vn as narrow band]
				setState(idxNei[j], VOXEL_NARROW_BAND);
				// [insert (d,vn) in Heap]
				heap.insert( getPointerVoxel(idxNei[j]), Value(idxNei[j]) );
			} else {
				// [decrease key of vn in Heap to d]
				heap.update( getPointerVoxel(idxNei[j]), Value(idxNei[j]) );
			}
		}
	}
}

void DistanceField::getNeighbour(Vector3i index, int &rCan, Vector3i *&resIdxNei) {
	int i, j;
	rCan = 0;
	resIdxNei = new Vector3i[6];
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 3; j++) {
			resIdxNei[i][j] = index[j];
		}
	}

	for (i = 0; i < 3; i++) {
		// x+ y= z=
		if (index[i] + 1 < tam[i]) {
			resIdxNei[rCan][i] = index[i] + 1;
			rCan++;
		}
		// x- y= z=
		if (index[i] - 1 >= 0) {
			resIdxNei[rCan][i] = index[i] - 1;
			rCan++;
		}
	}
}

bool DistanceField::ComputeDistance(Vector3i idx, float &maxSol) {
	int		i, j;
	float	coeff[3] = {-1, 0, 0};
	float	val1, val2, _val1, _val2;
	Vector3i	pni, pni2;

	for (j = 0; j < 3; j++) {
		val1 = FLT_MAX;
		val2 = FLT_MAX;

		for (i = 0; i < 2; i++) {
			pni = idx + idxN6[2*j+i];
			if ( !isIndexValid(pni) ) continue;

			if (State(pni) == VOXEL_FROZEN) {
				_val1 = Value(pni);

				if (_val1 < val1) {
					val1 = _val1;
					pni2 = pni + idxN6[2*j+i]*2;
					if ( !isIndexValid(pni2) ) { 
						val2 = FLT_MAX;
						continue;
					}
					_val2 = Value(pni2);

					if (State(pni2) == VOXEL_FROZEN && _val2 <= _val1)
						val2 = _val2;
					else
						val2 = FLT_MAX;
				}
			}
		}

		if (HIGH_ACCURACY && val2 != FLT_MAX) {
			float tp = (1.f/3.f) * (4.f*val1 - val2);
			float a = 9.f/4.f;
			coeff[2] += a;
			coeff[1] -= 2 * a * tp;
			coeff[0] += a * tp * tp;
		} else if (val1 != FLT_MAX) {
			coeff[2] += 1;
			coeff[1] -= 2 * val1;
			coeff[0] += val1 * val1;
		}
	}
	maxSol = FLT_MAX;
	float sol[2] = {FLT_MAX, FLT_MAX};
	if (int nroSol = SolveQuadric(coeff, sol)) {
		if (nroSol == 2)
			maxSol = (float)max(sol[1], sol[0]);
		else 
			maxSol = (float)sol[0];
		return true;
	}
	return false;
}

int	DistanceField::SolveQuadric(float coeff[3], float sol[2]) {
	float a = coeff[2];
	float b = coeff[1];
	float c = coeff[0];
	float discr = b*b - 4*a*c;
	if (discr > 0) {
		float root_discr = sqrt(discr);
		sol[0] = (-b + root_discr) / (2*a);
		sol[1] = (-b - root_discr) / (2*a);
		return 2;
	}
	if (discr == 0) {
		sol[0] = -b / (2*a);
		return 1;
	}
	if (discr < 0) {
		// 2 numeros complexos conjugados
	}
	return 0;
}

void DistanceField::Loop() {
	int		j;
	int		canNei;
	float	dist;
	Vector3i	*idxNei;
	Voxel		*pVoxel;

	// [while Heap is not empty]
	while (!heap.empty()) {
		// [Extract v from top of Heap]
		pVoxel = heap.front_value();
		heap.pop();

		// [Freeze v]
		pVoxel->state = VOXEL_FROZEN;

		// [For each neighbour vn of v]  
		getNeighbour(pVoxel->index, canNei, idxNei);
		for (j = 0; j < canNei; j++) {
			// [if vn is not Frozen]
			if (State(idxNei[j]) == VOXEL_FROZEN)
				continue;

			// [Compute distance d at vn]
			if ( !ComputeDistance(idxNei[j], dist) )
				continue;

			// Update distMax
			if (dist > distMax) 
				distMax = dist;

			// Update only if the new value is less
			if ( dist >= Value(idxNei[j]) )
				continue;
			setValue(idxNei[j], dist);
			setInner(idxNei[j], pVoxel->inner);

			// [if vn is not in narrow band]
			if (State(idxNei[j]) == VOXEL_FAR) {
				// [tag vn as narrow band]
				setState(idxNei[j], VOXEL_NARROW_BAND);
				// [insert (d,vn) in Heap]
				heap.insert( getPointerVoxel(idxNei[j]), Value(idxNei[j]) );
			} else {
				// [decrease key of vn in Heap to d]
				heap.update( getPointerVoxel(idxNei[j]), Value(idxNei[j]) );
			}
		}
	}
}

void DistanceField::Process() {
	bool isObject = true;
	if (isObject){
		Init();
		// Voxelizar os triangulos
		VoxelizeTriangles();
		// voxelInitials tem a lista de punteros aos voxels iniciais
		Initialization(voxelInitials);
	} else {
		setWidthVoxel(1.0f);
		pos		= Vector3f(2.0f, 2.0f, 2.0f);
		tam		= Vector3i(10, 10, 10);
		Init();

		// Get Voxels initials
		vector<Voxel*> vVoxel;
		Voxel *tmp;
		// tmp = getPointerVoxel(Vector3i(2,1,2));
		tmp = getPointerVoxel(getIdxVoxelOfPoint(Vector3f(2.f,2.f,2.f)));
		tmp->distance = 0.0f;
		tmp->state = VOXEL_FROZEN;
		vVoxel.push_back(tmp);
		tmp = getPointerVoxel(getIdxVoxelOfPoint(Vector3f(14.f,13.f,11.f)));
		tmp->distance = 0.0f;
		tmp->state = VOXEL_FROZEN;
		vVoxel.push_back(tmp);

		// vVoxel tem a lista de punteros aos voxels iniciais
		Initialization(vVoxel);
	}

	// Comenzar o ciclo de calculo de campos de distancia
	Loop();
	getIsoSurface();
}

void DistanceField::Print() {
	int ix, iy, iz;
	for (ix = 0; ix < tam.x; ix++) {
		for (iy = 0; iy < tam.y; iy++) {
			for (iz = 0; iz < tam.z; iz++) {
				if (operationOn) {
					cout << boxCopy[ix][iy][iz].distance << " " ;
				} else {
					cout << box[ix][iy][iz].distance << " " ;
				}
			}
			cout << "\n";
		}
		cout << "\n\n";
	}
}

void DistanceField::getOpenGL() {
	int			idxColor;
	float		dist;
	Vector3i	i;
	Vector3f	color, posTmp;

	glPointSize(3.0f);
	glEnable(GL_POINT_SMOOTH);

	glBegin(GL_POINTS);
	for (i.x = 0; i.x < tam.x; i.x++) {
		for (i.y = 0; i.y < tam.y; i.y++) {
			for (i.z = 0; i.z < tam.z; i.z++) {
				if (operationOn) {
					dist = boxCopy[i.x][i.y][i.z].distance;
				} else {
					dist = Value(i);
				}
				idxColor = (int)floor(dist * 15 / distMax);
				if (idxColor > 15) idxColor = 15;
				if (idxColor < 0) idxColor = 0;
				color = colorTable[idxColor];
				// glColor3f(1 - dist/20, 1 - dist/20, 1.0f);
				glColor3f(color.r, color.g, color.b);
				posTmp = pos + (Vector3f)i * width + width2;
				// glVertex3f(pos.x + (float)ix*width, pos.y + (float)iy*width, pos.z + (float)iz*width);
				glVertex3f(posTmp.x, posTmp.y, posTmp.z);
			}
		}
	}
	glEnd();
}

void DistanceField::getPointsOpenGL() {
	int			idxColor;
	float		dist;
	Vector3i	i;
	Vector3f	color, posTmp;
	int			iv = 0, numVertices = tam.x*tam.y*tam.z;

	glPointSize(3.0f);
	glEnableClientState( GL_VERTEX_ARRAY );	 // Enable Vertex Arrays
	glEnableClientState( GL_COLOR_ARRAY );	 // Enable Vertex Arrays
	float	*vertices	= new float[3*numVertices];
	float	*colores	= new float[3*numVertices];

	for (i.x = 0; i.x < tam.x; i.x++) {
		for (i.y = 0; i.y < tam.y; i.y++) {
			for (i.z = 0; i.z < tam.z; i.z++) {
				dist = operationOn ? boxCopy[i.x][i.y][i.z].distance : Value(i);
				idxColor = (int)floor(dist * 15 / distMax);
				if (idxColor > 15) idxColor = 15;
				if (idxColor < 0) idxColor = 0;
				color	= colorTable[idxColor];
				posTmp	= pos + (Vector3f)i * width + width2;
				colores[iv]		= color.r;
				colores[iv+1]	= color.g;
				colores[iv+2]	= color.b;
				vertices[iv]	= posTmp.x;
				vertices[iv+1]	= posTmp.y;
				vertices[iv+2]	= posTmp.z;
				iv += 3;
			}
		}
	}
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colores);
	glDrawArrays(GL_POINTS, 0, numVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void DistanceField::getVoxelsOpenGL() {
	int			idxColor;
	float		dist;
	Vector3i	i;
	Vector3f	color, posTmp;
	
	for (i.x = 0; i.x < tam.x - 1; i.x++) {
		for (i.y = 0; i.y < tam.y - 1; i.y++) {
			for (i.z = 0; i.z < tam.z - 1; i.z++) {
				if (operationOn) {
					dist = boxCopy[i.x][i.y][i.z].distance;
				} else {
					dist = Value(i);
				}
				idxColor = (int)floor(dist * 15 / distMax);
				if (idxColor > 15) idxColor = 15;
				if (idxColor < 0) idxColor = 0;
				color = colorTable[idxColor];
				glColor3f(color.r, color.g, color.b);
				glBegin(GL_LINE_LOOP);
					posTmp = pos + (Vector3f)i * width + width2;
					glVertex3f(posTmp.x, posTmp.y, posTmp.z);
				glEnd();
			}
		}
	}
}

void DistanceField::getIsoSurface(bool copy) {
	MarchingCubes mc;
	MarchingCubes::GridCell cell;
	/*
	if (triangIsoSurface != NULL) {
		if (triangIsoSurfaceTmp != NULL) {
			for (int i = 0; i < triangIsoSurfaceTmp->size(); i++) {
				if ( (*triangIsoSurfaceTmp)[i] != NULL ) {
					delete (*triangIsoSurfaceTmp)[i];
				}
			}
			delete triangIsoSurfaceTmp;
		}
		triangIsoSurfaceTmp = triangIsoSurface;
	}
	*/
	triangIsoSurface = new vector<Triangle*>;
	Triangle	*tmpTriang;
	int			k, nroTriang;
	Vector3i	i, j, tmplIdx[8];

	tmplIdx[0] = Vector3i(0,0,0);
	tmplIdx[1] = Vector3i(1,0,0);
	tmplIdx[2] = Vector3i(1,0,1);
	tmplIdx[3] = Vector3i(0,0,1);
	tmplIdx[4] = Vector3i(0,1,0);
	tmplIdx[5] = Vector3i(1,1,0);
	tmplIdx[6] = Vector3i(1,1,1);
	tmplIdx[7] = Vector3i(0,1,1);

	for (i.x = 0; i.x < tam.x - 1; i.x++) {
		for (i.y = 0; i.y < tam.y - 1; i.y++) {
			for (i.z = 0; i.z < tam.z - 1; i.z++) {
				for (k = 0; k < 8; k++) {
					j = i + tmplIdx[k];
					// j is Vector3i but we need a Vector3f.
					cell.p[k]	= pos + (Vector3f)j * width + width2;
					if (copy) {
						cell.val[k] = boxCopy[j.x][j.y][j.z].distance;
					} else {
						cell.val[k] = Value(j);
					}
					/*
					cout << j << " = " << i << " + " << tmplIdx[k] << endl;
					cout << " j * width = " << (Vector3f)j * width << endl;
					cout << cell.p[k] << " = " << pos << " + " << j << " * " << width << " + " << width2 << endl;
					*/
				}
				nroTriang = mc.Polygonise(cell, isoValue, tmpTriang);
				// nroTriang = mc.Polygonise(cell, width2, tmpTriang);
				for (k = 0; k < nroTriang; k++) {
					triangIsoSurface->push_back(&tmpTriang[k]);
				}
			}
		}
	}

}

void DistanceField::getIsoSurfaceOpenGL() {
	unsigned int	i;
	Triangle *pTri;

	// glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < triangIsoSurface->size(); i++) {
		pTri = (*triangIsoSurface)[i];
		glNormal3f(pTri->normal.x, pTri->normal.y, pTri->normal.z);
		glVertex3f(pTri->v[0].x, pTri->v[0].y, pTri->v[0].z);
		glNormal3f(pTri->normal.x, pTri->normal.y, pTri->normal.z);
		glVertex3f(pTri->v[1].x, pTri->v[1].y, pTri->v[1].z);
		glNormal3f(pTri->normal.x, pTri->normal.y, pTri->normal.z);
		glVertex3f(pTri->v[2].x, pTri->v[2].y, pTri->v[2].z);
	}
	glEnd();
}

/* OPERATIONS */
void DistanceField::OperationWaveAbove() {
	if (!operationOn) {
		operationOn = true;
		CopyBox();
		nivel = 0;
	}
	nivel++;
	// Incrementar el campo de distancia a partir de arriba en hasta el valor de nivel
	IncreaseFromAbove(nivel, 1.0f);
	// Get IsoSurface
	getIsoSurface(true);

}

void DistanceField::IncreaseFromAbove(int canNivel, float canIncrease) {
	Vector3i i;
	for (i.y = tam.y - 1; i.y >= 0 && tam.y - i.y <= canNivel; i.y--) {
		for (i.x = 0; i.x < tam.x; i.x++) {
			for (i.z = 0; i.z < tam.z; i.z++) {
				boxCopy[i.x][i.y][i.z].distance += canIncrease;
			}
		}
	}
}

void DistanceField::OperationCooking() {
	if (!operationOn) {
		operationOn = true;
		CopyBox();
		//nivel = tam.y;
	}
	//nivel--;
	Vector3i i;

	i.y = 0;
	for (i.x = 0; i.x < tam.x; i.x++) {
		for (i.z = 0; i.z < tam.z; i.z++) {
			boxCopy[i.x][i.y][i.z].distance = (boxCopy[i.x][i.y][i.z].distance < boxCopy[i.x][i.y+1][i.z].distance) ? boxCopy[i.x][i.y][i.z].distance : boxCopy[i.x][i.y+1][i.z].distance;
		}
	}
	for (i.y = 1; i.y < tam.y - 1 ; i.y++) {
		for (i.x = 0; i.x < tam.x; i.x++) {
			for (i.z = 0; i.z < tam.z; i.z++) {
				boxCopy[i.x][i.y][i.z].distance = boxCopy[i.x][i.y+1][i.z].distance;
			}
		}
	}
	i.y = tam.y-1;
	float dist;
	for (i.x = 0; i.x < tam.x; i.x++) {
		for (i.z = 0; i.z < tam.z; i.z++) {
			if ( !ComputeDistance(i, dist) )
				continue;
			boxCopy[i.x][i.y][i.z].distance = dist;
		}
	}
	// Print();
	getIsoSurface(true);
}

void DistanceField::OperationElastic() {
	if (!operationOn) {
		operationOn = true;
		CopyBox();
		nivel = tam.y;
	}
	if (nivel <= 2) 
		return;

	nivel--;
	Vector3i i;

	for (i.y = 0; i.y < nivel - 2 ; i.y++) {
		for (i.x = 0; i.x < tam.x; i.x++) {
			for (i.z = 0; i.z < tam.z; i.z++) {
				boxCopy[i.x][i.y][i.z].distance = (boxCopy[i.x][i.y][i.z].distance < boxCopy[i.x][i.y+1][i.z].distance) ? boxCopy[i.x][i.y][i.z].distance : boxCopy[i.x][i.y+1][i.z].distance;
			}
		}
	}
	i.y = nivel - 1;
	for (i.x = 0; i.x < tam.x; i.x++) {
		for (i.z = 0; i.z < tam.z; i.z++) {
			boxCopy[i.x][i.y][i.z].distance = boxCopy[i.x][i.y+1][i.z].distance;
		}
	}
	// Print();
	getIsoSurface(true);
}

void DistanceField::OperationErosion() {
	if (!operationOn) {
		operationOn = true;
		CopyBox();
		//nivel = tam.y;
	}
	//nivel--;
	Vector3i i;

	for (i.y = 0; i.y < tam.y; i.y++) {
		for (i.x = 0; i.x < tam.x; i.x++) {
			for (i.z = 0; i.z < tam.z; i.z++) {
				boxCopy[i.x][i.y][i.z].distance += 0.01f;
			}
		}
	}
	// Print();
	getIsoSurface(true);
}

void DistanceField::OperationDilation() {
	if (!operationOn) {
		operationOn = true;
		CopyBox();
		//nivel = tam.y;
	}
	//nivel--;
	Vector3i i;

	for (i.y = 0; i.y < tam.y; i.y++) {
		for (i.x = 0; i.x < tam.x; i.x++) {
			for (i.z = 0; i.z < tam.z; i.z++) {
				boxCopy[i.x][i.y][i.z].distance -= 0.01f;
			}
		}
	}
	// Print();
	getIsoSurface(true);
}

/* Voxelization initial*/
void DistanceField::VoxelizeTriangles() {
	if (triangles == NULL) return;
	int		i;
	float	dist;
	Triangle	*tri;
	Vector3i	j, idx[3], iInf, iSup;
	Vector3f	point;
	Voxel		*vxl, tmpVxl;

	// [For each triangle]
	for (i = 0; i < (int)triangles->size(); i++) {
		tri = (*triangles)[i];
		tri->D = -( tri->normal.dotProduct(tri->v[0]) );

		// Procurar os indexes de cada ponto
		idx[0] = getIdxVoxelOfPoint(tri->v[0]);
		idx[1] = getIdxVoxelOfPoint(tri->v[1]);
		idx[2] = getIdxVoxelOfPoint(tri->v[2]);

		// Si alguno de los tres indices no es valio, continuar con el siguiente triangulo.
		if (!isIndexValid(idx[0]) || !isIndexValid(idx[1]) || !isIndexValid(idx[2]) ) continue;

		// Encontrar el subcubo
		iInf.x = min(idx[0].x, min(idx[1].x, idx[2].x));
		iInf.y = min(idx[0].y, min(idx[1].y, idx[2].y));
		iInf.z = min(idx[0].z, min(idx[1].z, idx[2].z));
		iSup.x = max(idx[0].x, max(idx[1].x, idx[2].x));
		iSup.y = max(idx[0].y, max(idx[1].y, idx[2].y));
		iSup.z = max(idx[0].z, max(idx[1].z, idx[2].z));

		// Para cada voxel dentro do subcubo calcular a distancia ao triangulo
		for (j.x = iInf.x; j.x <= iSup.x; j.x++) {
			for (j.y = iInf.y; j.y <= iSup.y; j.y++) {
				for (j.z = iInf.z; j.z <= iSup.z; j.z++) {
					point = pos + (Vector3f)j * width + width2;
					dist = getDistancePointTriangle(point, tri, tmpVxl);

					if (dist <= distWidth2) {
						vxl = getPointerVoxel(j);
						vxl->distance	= dist;
						vxl->inner		= tmpVxl.inner;
						if (vxl->state != VOXEL_FROZEN) {
							vxl->state	= VOXEL_FROZEN;
							voxelInitials.push_back(vxl);
						}
					}
				}
			}
		}
	}
}

Vector3i DistanceField::getIdxVoxelOfPoint(Vector3f &p) {
	Vector3i idx;
	idx.x = (int)floor( abs(p.x - pos.x) / width );
	idx.y = (int)floor( abs(p.y - pos.y) / width );
	idx.z = (int)floor( abs(p.z - pos.z) / width );
	return idx;
}

float DistanceField::getDistancePointTriangle(Vector3f &p, Triangle *&tri, Voxel &vxl) {
	float distPlane = tri->normal.dotProduct(p) + tri->D;
	vxl.inner = (distPlane < 0 ? true : false);
	// Encontrar pI: projeccion del punto p en el plano del triangulo.
	Vector3f pI = p - tri->normal * distPlane;
	// Procurar em qual regiao fica pI
	Vector3f w0 = tri->v[0] - pI;
	Vector3f w1 = tri->v[1] - pI;
	Vector3f w2 = tri->v[2] - pI;

	Vector3f n02 = w0.crossProduct(w2);
	Vector3f n01 = w0.crossProduct(w1);
	Vector3f n12 = w1.crossProduct(w2);

	int sn02, sn01, sn12;
	sn02 = ( (tri->normal.x >= 0 && n02.x >= 0) || (tri->normal.x < 0 && n02.x < 0) ? 1 : -1);
	sn02 = ( (tri->normal.y >= 0 && n02.y >= 0) || (tri->normal.y < 0 && n02.y < 0) ? 1 : -1);
	sn02 = ( (tri->normal.z >= 0 && n02.z >= 0) || (tri->normal.z < 0 && n02.z < 0) ? 1 : -1);

	sn01 = ( (tri->normal.x >= 0 && n01.x >= 0) || (tri->normal.x < 0 && n01.x < 0) ? 1 : -1);
	sn01 = ( (tri->normal.y >= 0 && n01.y >= 0) || (tri->normal.y < 0 && n01.y < 0) ? 1 : -1);
	sn01 = ( (tri->normal.z >= 0 && n01.z >= 0) || (tri->normal.z < 0 && n01.z < 0) ? 1 : -1);

	sn12 = ( (tri->normal.x >= 0 && n12.x >= 0) || (tri->normal.x < 0 && n12.x < 0) ? 1 : -1);
	sn12 = ( (tri->normal.y >= 0 && n12.y >= 0) || (tri->normal.y < 0 && n12.y < 0) ? 1 : -1);
	sn12 = ( (tri->normal.z >= 0 && n12.z >= 0) || (tri->normal.z < 0 && n12.z < 0) ? 1 : -1);

	int R;
	// Procurar em qual regiao fica pI, usando os signos de n02, n01 e n12	
	if		(sn02 >= 0 && sn01 <  0 && sn12 >= 0) { R = 0; }
	else if (sn02 <  0 && sn01 <  0 && sn12 >= 0) { R = 1; }
	else if (sn02 <  0 && sn01 <  0 && sn12 <  0) { R = 2; }
	else if (sn02 <  0 && sn01 >= 0 && sn12 <  0) { R = 3; }
	else if (sn02 >= 0 && sn01 >= 0 && sn12 <  0) { R = 4; }
	else if (sn02 >= 0 && sn01 >= 0 && sn12 >= 0) { R = 5; }
	else if (sn02 <  0 && sn01 >= 0 && sn12 >= 0) { R = 6; }
	//
	else if (sn02 >= 0 && sn01 <  0 && sn12 <  0) { R = 6; }

	// Calcular la distancia segun su region
	// Distancia de dos puntos R0, R2 e R4
	if (R == 0) { 
		vxl.distance	= w0.length();
		//vxl.pointInter	= tri.v[0];
		return vxl.distance;
	}
	if (R == 2) { 
		vxl.distance	= w1.length();
		//vxl.pointInter	= tri.v[1];
		return vxl.distance;
	}
	if (R == 4) {
		vxl.distance	= w2.length();
		//vxl.pointInter	= tri.v[2];
		return vxl.distance;
	}
	// Distancia de punto a linea: R1, R3 e R5
	Vector3f ve(0,0,1);
	if (R == 1) { return getDistancePointLine(p, tri->v[0], tri->v[1]); }
	if (R == 3) { return getDistancePointLine(p, tri->v[1], tri->v[2]); }
	if (R == 5) { return getDistancePointLine(p, tri->v[0], tri->v[2]); }
	// Distancia de punto a plano: R6
	if (R == 6) {
		vxl.distance	= distPlane;
		//vxl.pointInter	= pI;
		return vxl.distance;
	}
	return vxl.distance;
}

float DistanceField::getDistancePointLine(Vector3f &p, Vector3f &p0, Vector3f &p1) {
	Vector3f v_p0_p		= p - p0;
	Vector3f v_p0_p1	= p1 - p0;
	return v_p0_p.crossProduct(v_p0_p1).length() / v_p0_p1.length();
}

void DistanceField::CopyBox() {
	int ix, iy, iz;
	boxCopy = new Voxel**[tam.x];
	for (ix = 0; ix < tam.x; ix++) {
		boxCopy[ix] = new Voxel*[tam.y];
		for (iy = 0; iy < tam.y; iy++) {
			boxCopy[ix][iy] = new Voxel[tam.z];
			for (iz = 0; iz < tam.z; iz++) {
				boxCopy[ix][iy][iz] = box[ix][iy][iz];
			}
		}
	}
}

Vector3f DistanceField::colorTable[16] = {
	Vector3f(1.0f, 0.0f, 0.0f),
	Vector3f(1.0f, 0.2f, 0.0f),
	Vector3f(1.0f, 0.4f, 0.0f),
	Vector3f(1.0f, 0.6f, 0.0f),
	Vector3f(1.0f, 0.8f, 0.0f),
	Vector3f(1.0f, 1.0f, 0.0f),
	Vector3f(0.8f, 1.0f, 0.0f),
	Vector3f(0.6f, 1.0f, 0.0f),
	Vector3f(0.4f, 1.0f, 0.0f),
	Vector3f(0.2f, 1.0f, 0.0f),
	Vector3f(0.0f, 1.0f, 0.0f),
	Vector3f(0.1f, 0.9f, 0.0f),
	Vector3f(0.2f, 0.8f, 0.0f),
	Vector3f(0.3f, 0.7f, 0.0f),
	Vector3f(0.4f, 0.6f, 0.0f),
	Vector3f(0.5f, 0.5f, 0.0f),
};

#endif
