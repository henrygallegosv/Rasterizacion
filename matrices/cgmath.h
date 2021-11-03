//
// Created by henry on 28/10/2021.
//

#ifndef MODELOPLY_CGMATH_H
#define MODELOPLY_CGMATH_H

#include <iostream>
#include <cmath>
#include "vector.h"
using namespace std;

#define ANG2RAD 3.14159265358979323846/180.0

class mat4 {
public:
    float *mat;
    mat4() {
        mat = new float[16]();
    }
    void print() {
        for (int i =0; i<4;i++) {
            for (int j = 0; j< 4; j++)
                cout << mat[4*i + j] << " ";
            cout << "\n";
        }
    }


    void lookAt(double eyex, double eyey, double eyez,
                double centerx, double centery, double centerz,
                double upx, double upy, double upz) {
        mat4 traslacion;
        traslacion.identity();
        traslacion.traslacion(eyex, eyey, eyez);
        vec3f vecz(eyex - centerx, eyey - centery, eyez - centerz);
        vecz.normalize();
        vec3f up(upx, upy, upz);
        vec3f vecx = up.productoCruz( vecz );
        vecx.normalize();
        vec3f vecy = vecz.productoCruz( vecx );


        mat[0] = vecx.x;
        mat[1] = vecx.y;
        mat[2] = vecx.z;

        mat[4] = vecy.x;
        mat[5] = vecy.y;
        mat[6] = vecy.z;

        mat[8] = vecz.x;
        mat[9] = vecz.y;
        mat[10] = vecz.z;

        mat[15] = 1.;
        multiplicacion( traslacion );
    }






    void identity(){
        for (int i=0; i<16; i++)
            mat[i] = 0.0;
        for (int i=0; i<4; i++)
            mat[4*i+i] = 1.0;
    }
    void traslacion(float tx, float ty, float tz) {
        mat[4*0+3] = tx;
        mat[4*1+3] = ty;
        mat[4*2+3] = tz;
    }
    void escala(float sx, float sy, float sz) {
        mat[4*0+0] = sx;
        mat[4*1+1] = sy;
        mat[4*2+2] = sz;
    }
    void rotacion(float gx, float gy, float gz) {
        mat4 tmpX;
        tmpX.rotacionX(ANG2RAD*gx);
        multiplicacion(tmpX);
    }

    void multiplicacion(mat4 &m) {
        float *tmp = new float[16];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                tmp[j*4 + i] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    tmp[j*4 + i] += mat[k*4 + i] * m.mat[j*4 + k];
                }
            }
        }
        delete [] mat;
        mat = tmp;
    }

    void rotacionX(float gx){
        identity();
        mat[5] = cos(gx);
        mat[6] = -sin(gx);
        mat[9] = sin(gx);
        mat[10] = cos(gx);
    }

};


#endif //MODELOPLY_CGMATH_H
