#ifndef _VOXEL_H_
#define _VOXEL_H_

#define VOXEL_FAR			0
#define VOXEL_FROZEN		1
#define VOXEL_NARROW_BAND	2

#include "vmath.h"

class Voxel {
public:
	static	float	tam;
	Vector3i	index;		// index of the voxel in the distance field
	//float		point[3];	// x, y, z
	float		distance;	// distance to the object
	// Vector3f	pointInter;	// Point of intersection
	bool		inner;		// If the point is inner or extern
	int			state;		// VOXEL_FAR, VOXEL_FROZEN or VOXEL_NARROW_BAND

	Voxel() {
		//point[0] = point[1] = point[2] = 0.0;
		setIndex(0, 0, 0);
		state = VOXEL_FAR;
	}
	Voxel(const Voxel &v);

	Voxel(int x, int y, int z) {
		setIndex(x, y, z);
		state = VOXEL_FAR;
	}
	Voxel(float x, float y, float z) {
		//setPoint(x, y, z);
	}
	/*
	void setPoint(float x, float y, float z) {
		point[0] = x;
		point[1] = y;
		point[2] = z;
	}
	*/
	void setIndex(int ix, int iy, int iz) {
		index[0] = ix;
		index[1] = iy;
		index[2] = iz;
	}

	Voxel	clone();
	
};

Voxel::Voxel(const Voxel &v) {
	distance = v.distance;
	index = v.index;
	inner = v.inner;
	state = v.state;
}

Voxel Voxel::clone(){
	Voxel *copy = new Voxel();
	copy->distance = distance;
	copy->index = index;
	copy->inner = inner;
	copy->state = state;
	return *copy;
}

//Overload the < operator.
bool operator< (const Voxel &vxl1, const Voxel &vxl2)
{
	//return vxl1.distance > vxl2.distance;	
	if (vxl1.index.x < vxl2.index.x) return true;
	if (vxl1.index.y < vxl2.index.y) return true;
	if (vxl1.index.z < vxl2.index.z) return true;
	return false;
}
//Overload the > operator.
bool operator> (const Voxel &vxl1, const Voxel &vxl2)
{
	//return vxl1.distance < vxl2.distance;	
	if (vxl1.index.x > vxl2.index.x) return true;
	if (vxl1.index.y > vxl2.index.y) return true;
	if (vxl1.index.z > vxl2.index.z) return true;
	return false;
}
//Overload the == operator.
bool operator== (const Voxel &vxl1, const Voxel &vxl2)
{
	if (vxl1.index.x != vxl2.index.x) return false;
	if (vxl1.index.y != vxl2.index.y) return false;
	if (vxl1.index.z != vxl2.index.z) return false;
	return true;
}

/*
//Overload the < operator.
bool operator< (const Voxel *&vxl1, const Voxel *&vxl2)
{
	return vxl1->distance > vxl2->distance;	
}
//Overload the > operator.
bool operator> (const Voxel *&vxl1, const Voxel *&vxl2)
{
	return vxl1->distance < vxl2->distance;	
}
//Overload the == operator.
bool operator== (const Voxel *&vxl1, const Voxel *&vxl2)
{
	return vxl1->index == vxl2->index;	
}
*/


#endif
