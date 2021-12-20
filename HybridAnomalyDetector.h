//Ido Tavron 316222512
//Tal Dabran 316040898
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual void pickMethod(correlatedFeatures* cor, float curPearson, float upThresh, float lowThresh,
                            int arraySize, string it2, Point** pointArr, Point* p,
                            float* array1, float* array2);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */