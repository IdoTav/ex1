
#include "HybridAnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#define TOP_THERSHOLD 0.9
#define BOT_THERSHOLD 0.5

HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
}

void HybridAnomalyDetector::pickMethod(correlatedFeatures *cor, float curPearson, float upThresh, float lowThresh,
                                     int arraySize, string it2, Point **pointArr, Point *p, float *array1,
                                     float *array2) {
    if (curPearson > lowThresh) {
        cor->feature2 = it2;
        cor->corrlation = curPearson;
        for (int i = 0; i < arraySize; i++) {
            p[i] = Point(array1[i], array2[i]);
        }
        initPointsArray(arraySize, pointArr, p);
        if (curPearson >= upThresh) {
            cor->lin_reg = linear_reg(pointArr, arraySize);
            // If we get into this condition we don't care about the circle
            cor->min_cir = Circle();
            upThresh = curPearson;
        } else {
            // Case the best correlation we find is more than 0.5 but less than 0.9
            cor->min_cir = findMinCircle(pointArr, arraySize);
            lowThresh = curPearson;
        }
    }
}
