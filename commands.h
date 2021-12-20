

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;
TimeSeries ts();
HybridAnomalyDetector ad();
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

// implement here your command classes

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
