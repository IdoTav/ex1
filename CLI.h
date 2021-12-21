

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "HybridAnomalyDetector.h"
#include "AnomalyDetector.h"

class DefaultIO;
using namespace std;

class CLI {
	DefaultIO* dio;
    TimeSeries* trainTs;
    TimeSeries* testTs;
    HybridAnomalyDetector ad;
    vector<AnomalyReport>* r;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
    void setTestTs(DefaultIO* dio);
    void setTrainTs(DefaultIO* dio);
    virtual ~CLI();
};

#endif /* CLI_H_ */
