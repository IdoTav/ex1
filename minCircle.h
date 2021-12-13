
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
    Point center;
    float radius;
    Circle():center(Point(0,0)),radius(0){}
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);
float find_the_distance(const Point& p1, const Point& p2);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
