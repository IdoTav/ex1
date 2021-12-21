//Ido Tavron 316222512
//Tal Dabran 316040898
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "HybridAnomalyDetector.h"
#include "AnomalyDetector.h"

class DefaultIO;
using namespace std;

class CLI {
	DefaultIO* dio;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
    void setTestTs(DefaultIO* dio);
    void setTrainTs(DefaultIO* dio);
    void setSettings(DefaultIO* dio);
    void runDetect(DefaultIO* dio);
    void runDisplay(DefaultIO* dio);
    void runAnalyze(DefaultIO* dio);
    virtual ~CLI();
};

#endif /* CLI_H_ */
