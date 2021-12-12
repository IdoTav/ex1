// Ido Tavron 316222512
// Tal Dabran 316040898
#include "minCircle.h"
#include "anomaly_detection_util.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <vector>
const double INF = 1e18;

/**
 * this function dist calculate the distance between two points
 * @param x is a point that we are want to check the distance between the two points
 * @param y is a point that we are want to check the distance between the two points
 */
float find_the_distance(const Point& p1, const Point& p2) {
    return sqrt(pow(p1.x - p2.x, 2)
                + pow(p1.y - p2.y, 2));
}

/**
 * this function Function to check whether a point lies inside or on the boundaries of the circle
 * @param circle is the circle that we are want to check the distance between the center to the point p with the radius
 * @param point is the point that we are want to check the distance between the center with the radius
 */
bool check_if_inside(const Circle& circle, const Point& point) {
    return find_the_distance(circle.center, point) <= circle.radius;
}

/**
 * The following function are used To find the equation of the circle when three points are given.
 * @param bx is a float that indicate the x of the first point
 * @param by is a float that indicate the y of the first point
 * @param cx is a float that indicate the x of the second point
 * @param cy is a circle that indicate the y of the second point
 */
Point find_circle_center(float bx, float by, float cx, float cy) {
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    Point point = Point((cy * B - by * C) / (2 * D),(bx * C - cx * B) / (2 * D));
    return point;
}

/**
 * The following Function to return a unique circle that intersects three points
 * @param A is a Point that with the point we will get the circle center
 * @param B is a Point that with the point we will get the circle center
 * @param C is a Point that with the point we will get the circle center
 */
Circle find_circle(const Point& A, const Point& B, const Point& C) {
    Point I = find_circle_center(B.x - A.x, B.y - A.y,C.x - A.x, C.y - A.y);
    I.x += A.x;
    I.y += A.y;
    Circle circle = Circle(I, find_the_distance(I, A));
    return  circle;
}

/**
 * The following Function to return the smallest circle that intersects 2 points
 * @param A is a Point that with this point we will get a circle
 * @param B is a Point that with this point we will get a circle
 */
Circle get_circle(const Point& A, const Point& B) {
    Point C = { (A.x + B.x) / 2, (A.y + B.y) / 2 };
    Circle circle = Circle(C, find_the_distance(A, B) / 2);
    return circle;
}

/**
 * The following Function is to check whether a circle encloses the given points
 * @param c is a circle that we are check if he is a valid circle
 * @param P is a POint vector with all the points
 */
bool check_if_valid(const Circle& c, const vector<Point>& P) {
    for (const Point& p : P)
        if (!check_if_inside(c, p))
            return false;
    return true;
}


/**
 * The following Function to return the minimum enclosing circle for N <= 3
 * @param P is a POint vector with all the points
 */
Circle find_3_or_low_circle(vector<Point>& P) {
    assert(P.size() <= 3);
    if (P.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (P.size() == 1) {
        return { P[0], 0 };
    }
    else if (P.size() == 2) {
        return get_circle(P[0], P[1]);
    }

    // To check if MEC can be determined
    // by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = get_circle(P[i], P[j]);
            if (check_if_valid(c, P))
                return c;
        }
    }
    return find_circle(P[0], P[1], P[2]);
}


/**
 * The following Function Returns the MEC using Welzl's algorithm takes a set of input points P and a set R points
                 on the circle boundary.
 * @param P is a Point vector with all the points
 * @param R is a Point vector
 * @oaram n is the number of points in the vector p
 */
Circle find_the_min_circle(vector<Point>& P, vector<Point> R, int n) {
    if (n == 0 || R.size() == 3) {
        return find_3_or_low_circle(R);
    }
    int idx = rand() % n;
    Point p = P[idx];
    swap(P[idx], P[n - 1]);
    Circle d = find_the_min_circle(P, R, n - 1);
    if (check_if_inside(d, p)) {
        return d;
    }
    R.push_back(p);
    return find_the_min_circle(P, R, n - 1);
}

/**
 * The following Function calls the welzl_
 * @param P is a Point vector with all the points
 * @param R is a Point vector
 * @oaram n is the number of points in the vector p
 */
Circle findMinCircle(Point** points, size_t size) {
    vector<Point> pointsVector(size);
    for (int i = 0; i < size; i++)
        pointsVector[i] = *(*(points + i));
    random_shuffle(pointsVector.begin(), pointsVector.end());
    return find_the_min_circle(pointsVector, {}, pointsVector.size());
}
