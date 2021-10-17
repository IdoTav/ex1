#include <iostream>
#include <complex>
#include "anomaly_detection_util.h"

float avg(float* x, int size) {
    double sum = 0;
    for (int i = 0; i <= size - 1; i++) {
        sum += x[i];
    }
    return float(sum / size);
}

float avgPow(float* x, int size, int exp) {
    double sum = 0;
    for (int i = 0; i <= size - 1; i++) {
        sum += std::pow(x[i], exp);
    }
    return float(sum / size);
}

// returns the variance of X and Y
float var(float* x, int size) {
    double m = avg(x, size), sum = avgPow(x, size, 2);
    return float(sum - std::pow(m,2));
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
    return std::abs(y - p.y);
}

int main () {
    Point p1 (1,2), p2 (2,2), p3 (3,3);
    Point* ptrArr[3];
    ptrArr[0] = &p1;
    ptrArr[1] = &p2;
    ptrArr[2] = &p3;
    std:: cout << linear_reg(ptrArr,3).a;
}