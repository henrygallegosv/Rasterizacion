#ifndef RT1_VECTOR_H
#define RT1_VECTOR_H

#include <cmath>
#include <ostream>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class vec3f {
public:
    float x, y, z;
    vec3f() {x=0, y=0, z=0;}

    vec3f(float _x, float _y, float _z):x{_x}, y{_y}, z{_z}{}

    void set(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    vec3f productoCruz(vec3f v);
    float productoPunto(vec3f v) {
        return x*v.x + y*v.y + z*v.z;
    }

    vec3f operator+(vec3f v);
    vec3f operator-(vec3f v);

    vec3f operator*(vec3f v);
    vec3f operator*(float f);
    vec3f operator/(float f);

    void operator*=(vec3f v);
    void operator+=(vec3f v);
    vec3f operator-(){ return vec3f(-x,-y,-z);}

    void normalize();
    double modulo();

    void max_to_one();

    vec3f yzx() { return vec3f(y, z, x); }
    vec3f xyz() { return vec3f(x, y, z); }
    vec3f zxy() { return vec3f(z, x, y); }

};
std::ostream& operator<<(std::ostream &os, vec3f v);

vec3f operator/(float f, vec3f &v);

vec3f abs(vec3f &v);
vec3f sign(vec3f &v);
vec3f step(vec3f v1, vec3f v2);

#endif //RT1_VECTOR_H
