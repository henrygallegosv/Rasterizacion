#ifndef _PLANE_H_
#define _PLANE_H_

#include "vmath.h"

class Plane {
public:
	Vector3f	point;
	Vector3f	normal;
	float		D;

	void	setNormalAndPoint(Vector3f &n, Vector3f &p);
	float	distance(Vector3f &p);
	// Projecting a point to a plane
};

void Plane::setNormalAndPoint(Vector3f &n, Vector3f &p) {
	point = p;
	normal = n;
	D = -n.dotProduct(p);
}

float Plane::distance(Vector3f &p) {
	return normal.dotProduct(p) + D;
}

#endif
