#ifndef _AABB_H_
#define _AABB_H_

#include "vmath.h"

class AABB {
public:
	Vector3f corner;
	float x, y, z;

	AABB() {
		corner.x = corner.y = corner.z = 0.0f;
		x = y = z = 1.0f;
	}
	AABB(Vector3f &corner, float x, float y, float z) {
		setBox(corner, x, y, z);
	}

	void setBox(Vector3f &corner, float x, float y, float z);

	// for use in frustum computations
	Vector3f getVertexP(Vector3f &normal);
	Vector3f getVertexN(Vector3f &normal);
};

void AABB::setBox(Vector3f &corner, float x, float y, float z) {
	this->corner = corner;
	if (x < 0.0) {
		x = -x;
		this->corner.x -= x;
	}
	if (y < 0.0) {
		y = -y;
		this->corner.y -= y;
	}
	if (z < 0.0) {
		z = -z;
		this->corner.z -= z;
	}
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3f AABB::getVertexP(Vector3f &normal) {
	Vector3f res = corner;
	if (normal.x > 0)
		res.x += x;
	if (normal.y > 0)
		res.y += y;
	if (normal.z > 0)
		res.z += z;
	return res;
}

Vector3f AABB::getVertexN(Vector3f &normal) {
	Vector3f res = corner;
	if (normal.x < 0)
		res.x += x;
	if (normal.y < 0)
		res.y += y;
	if (normal.z < 0)
		res.z += z;
	return(res);
}

#endif

