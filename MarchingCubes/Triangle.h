#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "vmath.h"
#include <math.h>

using namespace std;

class Triangle {
public:
    Vector3f	*v;
    Vector3f	normal;
	float		D;

    Triangle();
    Triangle(Vector3f v1, Vector3f v2, Vector3f v3);
	Triangle(Vector3f v1, Vector3f v2, Vector3f v3, Vector3f norm);

	void	setVertex(Vector3f v1, Vector3f v2, Vector3f v3);

    //AABB getAABB();
    void	printTriangle(int i = 0);
    //bool intersect(Triangle t2);
};

#endif
