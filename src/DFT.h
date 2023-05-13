// based on: https://www.geeksforgeeks.org/discrete-fourier-transform-and-its-inverse-using-c/

#ifndef DFT_FILTER
#define DFT_FILTER

#include "Arduino.h"

const int len = 10;
const int DFTlen = len;

float xi[len];
float Xr[len];
float Xi[len];

float xo[len];


void calculate_DFT(){
    
    for (int k = 0; k < DFTlen; k++) {
        Xr[k] = 0;
        Xi[k] = 0;
        for (int n = 0; n < len; n++) {
            Xr[k] = (Xr[k] + xi[n] * cos(2 * 3.141592 * k * n / DFTlen));
            Xi[k] = (Xi[k] - xi[n] * sin(2 * 3.141592 * k * n / DFTlen));
        }
    }
}
 

void calculate_IDFT(){
    int theta = 0;
    for (int n = 0; n < DFTlen; n++) {
        xo[n] = 0;
        for (int k = 0; k < DFTlen; k++) {
            theta = (2 * 3.141592 * k * n) / DFTlen;
            xo[n] = xo[n] + Xr[k] * cos(theta) + Xi[k] * sin(theta);
        }
        xo[n] = xo[n] / DFTlen;
    }
}
 
#endif