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

using namespace std;
TimeSeries trainTs;
TimeSeries testTs;
HybridAnomalyDetector ad;
vector<AnomalyReport> r;

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
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
};

void currentThreshold() {
    std::cout << "The current correlation threshold is" << ad.getTopThreshold() << std::endl;
    float newThreshold;
    std::cin >> newThreshold;
    while (newThreshold < 0 || newThreshold > 1) {
        std::cout <<"please choose a value between 0 and 1." << std::endl;
        std::cin >> newThreshold;
    }
    ad.setTopThreshold(newThreshold);
}

void uploadAtimeSeries() {
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
    trainTs = TimeSeries("anomalyTrain.csv");
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
    std:
    cout << "Upload complete" << std::endl;
    serverFile1.close();
    clientFile1.close();
    testTs = TimeSeries("anomalyTest.csv");
}

class displayCommand:public Command{
    DefaultIO* dio;
public:
    virtual void execute(){
        for (AnomalyReport ar : r) {
            std::cout << ar.timeStep + " " + ar.description << std::endl;
        }
    }
};



#endif /* COMMANDS_H_ */
