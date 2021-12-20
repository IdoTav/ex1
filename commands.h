#include <filesystem>
#include <iostream>
#include <fstream>
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "AnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Command{
	DefaultIO* dio;
protected:
    TimeSeries* trainTs;
    TimeSeries* testTs;
    HybridAnomalyDetector* ad;
    vector<AnomalyReport>* r;
public:
	// Command(DefaultIO* dio):dio(dio){}
    Command(TimeSeries* trainTsC, TimeSeries* testTsC, HybridAnomalyDetector* adC, vector<AnomalyReport>* rC):
    trainTs(trainTsC), testTs(testTsC), ad(adC), r(rC){}
    //Command(int l) {l = 6;}
	virtual void execute()=0;
	virtual ~Command(){}
};


class uploadAtimeSeriesCommand:public Command{
public:
    uploadAtimeSeriesCommand(TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                             vector<AnomalyReport> *rC) : Command(trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        std::cout << "Please upload your local test CSV file." << std::endl;
        std::ofstream serverFile("anomalyTrain.csv");
        std::ifstream clientFile;
        clientFile.open("trainFile.csv", ios::in);
        serverFile.open("anomalyTrain.csv", ios::out);
        string tp;
        while (getline(clientFile, tp)) {
            if (tp == "done")
                break;
            serverFile << tp;
        }
        serverFile.close();
        clientFile.close();
        TimeSeries tmp = TimeSeries("anomalyTrain.csv");
        trainTs = &tmp;
        std::cout << "Please upload your local train CSV file." << std::endl;
        std::ofstream serverFile1("anomalyTest.csv");
        std::ifstream clientFile1;
        clientFile1.open("testFile.csv", ios::in);
        serverFile1.open("anomalyTest.csv", ios::out);
        while (getline(clientFile1, tp)) {
            if (tp == "done")
                break;
            serverFile1 << tp;
        }
        std::cout << "Upload complete" << std::endl;
        serverFile1.close();
        clientFile1.close();
        TimeSeries tmp2 = TimeSeries("anomalyTest.csv");
        testTs = &tmp2;
    }
};


class detectAnomaliesCommand:public Command {
public:
    detectAnomaliesCommand(TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
    vector<AnomalyReport> *rC) : Command(trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        (*ad).learnNormal(*trainTs);
        *r = (*ad).detect(*testTs);
        std::cout << "anomaly detection complete." << std::endl;
    }
};

class currentThresholdCommand:public Command{
public:
    currentThresholdCommand(TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
    vector<AnomalyReport> *rC) : Command(trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        std::cout << "The current correlation threshold is" << " " << ad->getTopThreshold() << std::endl;
        float newThreshold;
        std::cin >> newThreshold;
        while (newThreshold < 0 || newThreshold > 1) {
            std::cout <<"please choose a value between 0 and 1." << std::endl;
            std::cin >> newThreshold;
        }
        ad->setTopThreshold(newThreshold);
    }
};

class displayCommand:public Command{
public:
    displayCommand(TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
    vector<AnomalyReport> *rC) : Command(trainTsC, testTsC, adC, rC) {}
    virtual void execute(){
        for (AnomalyReport ar : *r) {
            std::cout << to_string(ar.timeStep) << " " << ar.description << std::endl;
        }
    }
};

class analyzeCommand:public Command{
public:
    virtual void execute(){
        /*TODO - GET FILE FROM CLIENT*/
        vector<pair<long, long>> sumAnomalies;
        string desc;
        for (AnomalyReport ar : *r) {
            /*TODO - FIND ANOMALY IN RANGE*/
        }
    }
};

#endif /* COMMANDS_H_ */
