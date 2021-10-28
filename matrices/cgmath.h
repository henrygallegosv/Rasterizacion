//
// Created by henry on 28/10/2021.
//

#ifndef MODELOPLY_CGMATH_H
#define MODELOPLY_CGMATH_H

#include <iostream>
using namespace std;
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
    void indentity(){
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

};


#endif //MODELOPLY_CGMATH_H
