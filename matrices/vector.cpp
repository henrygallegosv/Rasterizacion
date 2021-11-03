#include "vector.h"
#include <iostream>

std::ostream& operator<<(std::ostream &os, vec3f v){
    os << "[" << v.x << ", " << v.y << "," << v.z << "]";
    return os;
}

vec3f operator/(float f, vec3f &v) {
    if (v.x == 0 || v.y == 0 || v.z == 0) {
        std::cout << "Hay cero";
    }
    float tx = (v.x == 0) ? f/0.01 : f/v.x;
    float ty = (v.y == 0) ? f/0.01 : f/v.y;
    float tz = (v.z == 0) ? f/0.01: f/v.z;

    return vec3f(tx, ty, tz);
}

vec3f abs(vec3f &v) {
    return vec3f( abs(v.x), abs(v.y), abs(v.z) );
}
vec3f sign(vec3f &v) {
    return vec3f( sgn(v.x), sgn(v.y), abs(v.z) );
}
vec3f step(vec3f v1, vec3f v2) {
    return vec3f(v2.x < v1.x ? 0 : 1,
                 v2.y < v1.y ? 0 : 1,
                 v2.z < v1.z ? 0 : 1);
}

vec3f vec3f::productoCruz(vec3f v) {
    return vec3f(y*v.z - v.y*z, v.x*z - x*v.z, x*v.y - v.x*y);
}

vec3f vec3f::operator+(vec3f v) {
    return vec3f(x + v.x, y +v.y, z+v.z);
}

vec3f vec3f::operator-(vec3f v) {
    return vec3f(x - v.x, y -v.y, z-v.z);
}

vec3f vec3f::operator*(vec3f v) { return vec3f(x*v.x, y*v.y, z*v.z); }

vec3f vec3f::operator*(float f) { return vec3f(f*x, f*y, f*z); }

vec3f vec3f::operator/(float f) { return vec3f(x/f, y/f, z/f); }

void vec3f::operator*=(vec3f v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
}
void vec3f::operator+=(vec3f v) {
    x += v.x;
    y += v.y;
    z += v.z;
}

void vec3f::normalize() {
    double m = modulo();
    x = x/m;
    y = y/m;
    z = z/m;
}

double vec3f::modulo() {
    return sqrt(x*x + y*y + z*z);
}

void vec3f::max_to_one() {
    float max_value = std::max(x, std::max(y,z));
    if (max_value > 1.0){
        x = x / max_value;
        y = y / max_value;
        z = z / max_value;
    }
}
