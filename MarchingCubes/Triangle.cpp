#include "Triangle.h"

Triangle::Triangle() {
    v = new Vector3f[3];
}

Triangle::Triangle(Vector3f v1, Vector3f v2, Vector3f v3) {
    v = new Vector3f[3];
    v[0] = Vector3f(v1.x, v1.y, v1.z);
    v[1] = Vector3f(v2.x, v2.y, v2.z);
    v[2] = Vector3f(v3.x, v3.y, v3.z);

    normal = (v[0] - v[2]).crossProduct(v[1] - v[2]);
	normal = -normal;
    normal.normalize();
}

void Triangle::setVertex(Vector3f v1, Vector3f v2, Vector3f v3) {
    v = new Vector3f[3];
    v[0] = Vector3f(v1.x, v1.y, v1.z);
    v[1] = Vector3f(v2.x, v2.y, v2.z);
    v[2] = Vector3f(v3.x, v3.y, v3.z);

    normal = (v[0] - v[2]).crossProduct(v[1] - v[2]);
    normal.normalize();
}

void Triangle::printTriangle(int i) {
    printf("%d: v1(%f,%f,%f), v2(%f,%f,%f), v3(%f,%f,%f) \n", i, (v[0])[0], (v[0])[1], (v[0])[2], (v[1])[0], (v[1])[1], (v[1])[2], (v[2])[0], (v[2])[1], (v[2])[2]);
}