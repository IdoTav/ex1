#include <iostream>
//Ido Tavron 316222512
//Tal Dabran 316040898
#include <complex>
#include "anomaly_detection_util.h"

float avg(float* x, int size) {
    double sum = 0;
    for (int i = 0; i <= size - 1; i++) {
        sum += x[i];
    }
    return float(sum / size);
}

float  localPow(float num, float exp) {
    //base case
    if (0 == exp)
        return 1;
    //case the exponent is negative
    if (0 > exp)
        return localPow(1/num, -exp);
    //case the exponent is fraction
    if (1 > exp && -1 < exp)
        return num * std::pow(num, exp);
    //regular case
    return num * localPow(num, exp-1);
}

float avgPow(float* x, int size, int exp) {
    double sum = 0;
    for (int i = 0; i <= size - 1; i++) {
        sum += localPow(x[i], (float)exp);
    }
    return float(sum / size);
}

// returns the variance of X and Y
float var(float* x, int size) {
    double m = avg(x, size), sum = avgPow(x, size, 2);
    return (float) sum - localPow((float) m,2);
}

void multiplyArray(float* x, float* y, float* c ,int size) {
    for (int i=0; i <= size - 1; i++) {
        c[i] = x[i] * y[i];
    }
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float c[size];
    multiplyArray(x, y,c ,size);
    return avg(c, size) - avg(x, size) * avg(y, size);
}

float pearson(float* x, float* y, int size) {
    float covTemp = cov(x,y,size), sDevX = std::sqrt(var(x,size)), sDevY= std::sqrt(var(y,size));
    return covTemp / (sDevX * sDevY);
}

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size) {
    float x[size], y[size], a, b;
    for (int i = 0; i<= size - 1; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - a * avg(x, size);
    Line l(a,b);
    return l;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size) {
    Line l = linear_reg(points, size);
    return dev(p, l);
}

float GetXFromLine(Point p, Line l) {
    return (p.y - l.b);
}

// returns the deviation between point p and the line
float dev(Point p,Line l) {
    float y = l.f(p.x);
    if (y < p.y) {
        return p.y - y;
    } else
        return y - p.y;
}