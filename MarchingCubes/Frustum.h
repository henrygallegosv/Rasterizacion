#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Plane.h"
#include "AABB.h"

#define ANG2RAD 3.14159265358979323846/180.0

class Frustum {
private:
	enum {TOP=0, BOTTOM, LEFT, RIGHT, NEARP, FARP};

public:
	static enum {OUTSIDE, INTERSECT, INSIDE};
	Plane pl[6];
	// n:near, f:far, t:top, b:bottom, l:left, r:right
	Vector3f ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nearWidth, nearHeight, farWidth, farHeight;

	Frustum::Frustum() {}
	Frustum::~Frustum() {}

	void	setCamInternals(float angle, float ratio, float nearD, float farD);
	void	setCamDef(Vector3f &p, Vector3f &l, Vector3f &u);
	int		pointInFrustum(Vector3f &p);
	int		sphereInFrustum(Vector3f &p, float radius);
	int		boxInFrustum(AABB &b);
};


// Each time the perspective definitions change, for instance when a window is resized, this function should be called as well.
void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD) {
	// store the information
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tang = (float)tan(ANG2RAD * angle * 0.5);
	nearHeight	= nearD * tang;
	nearWidth	= nearHeight * ratio;
	farHeight	= farD * tang;
	farWidth	= farHeight * ratio;
}

void Frustum::setCamDef(Vector3f &p, Vector3f &l, Vector3f &u) {
	Vector3f dir, nc, fc, X, Y, Z;

	// Compute the Z axis of camera
	// this axis points in the opposite direction from the looking direction
	Z = p - l;
	Z.normalize();

	// X axis of camera with given "up" vector and Z axis
	X = u.crossProduct(Z);
	X.normalize();

	// the real "up" vector is the cross product of Z and X
	Y = Z.crossProduct(X);

	// compute the centers of the near and far planes
	nc = p - Z * nearD;
	fc = p - Z * farD;

	/*	The function computed the eight corners of the frustum 
		and the six planes according to the algorithm defined
	// compute the 4 corners of the frustum on the near plane
	ntl = nc + Y * nearHeight - X * nearWidth;
	ntr = nc + Y * nearHeight + X * nearWidth;
	nbl = nc - Y * nearHeight - X * nearWidth;
	nbr = nc - Y * nearHeight + X * nearWidth;

	// compute the 4 corners of the frustum on the far plane
	ftl = fc + Y * farHeight - X * farWidth;
	ftr = fc + Y * farHeight + X * farWidth;
	fbl = fc - Y * farHeight - X * farWidth;
	fbr = fc - Y * farHeight + X * farWidth;

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order
	pl[TOP].set3Points(ntr,ntl,ftl);
	pl[BOTTOM].set3Points(nbl,nbr,fbr);
	pl[LEFT].set3Points(ntl,nbl,fbl);
	pl[RIGHT].set3Points(nbr,ntr,fbr);
	pl[NEARP].set3Points(ntl,ntr,nbr);
	pl[FARP].set3Points(ftr,ftl,fbl);
	*/

	/*	The following (more efficient) alternative may be used to replace 
		the computation of the eight corners and the six planes */
	pl[NEARP].setNormalAndPoint(-Z,nc);
	pl[FARP].setNormalAndPoint(Z,fc);

	Vector3f aux,normal;

	aux = (nc + Y*nearHeight) - p;
	aux.normalize();
	normal = aux.crossProduct(X);
	pl[TOP].setNormalAndPoint(normal, nc+Y*nearHeight);

	aux = (nc - Y*nearHeight) - p;
	aux.normalize();
	normal = X.crossProduct(aux);
	pl[BOTTOM].setNormalAndPoint(normal, nc-Y*nearHeight);

	aux = (nc - X*nearWidth) - p;
	aux.normalize();
	normal = aux.crossProduct(Y);
	pl[LEFT].setNormalAndPoint(normal, nc-X*nearWidth);

	aux = (nc + X*nearWidth) - p;
	aux.normalize();
	normal = Y.crossProduct(aux);
	pl[RIGHT].setNormalAndPoint(normal, nc+X*nearWidth);
}

int	Frustum::pointInFrustum(Vector3f &p) {
	int result = INSIDE;
	for (int i=0; i < 6; i++) {
		if ( pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return result;
}

int Frustum::sphereInFrustum(Vector3f &p, float radius) {
	float	distance;
	int		result = INSIDE;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -radius)
			return OUTSIDE;
		else if (distance < radius)
			result =  INTERSECT;
	}
	return result;
}

int	Frustum::boxInFrustum(AABB &b) {
	int result = INSIDE;
	for (int i = 0; i < 6; i++) {
		if ( pl[i].distance( b.getVertexP(pl[i].normal) ) < 0)
			return OUTSIDE;
		else if ( pl[i].distance( b.getVertexN(pl[i].normal) ) < 0 ) 
			result = INTERSECT;
	}
	return result;
}

#endif
