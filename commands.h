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
#include "CLI.h"

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
protected:
    DefaultIO* dio;
    CLI* _cli;
    TimeSeries* trainTs;
    TimeSeries* testTs;
    HybridAnomalyDetector* ad;
    vector<AnomalyReport>* r;
public:
    Command(DefaultIO* dio):dio(dio){}
    Command(DefaultIO* dio, CLI* cli):dio(dio), _cli(cli){}
    virtual void execute()=0;
    virtual ~Command(){}
};


class uploadAtimeSeriesCommand:public Command{
public:
    uploadAtimeSeriesCommand(DefaultIO* dio, CLI* cli) : Command(dio, cli) {}
    virtual void execute() {
        this->_cli->setTrainTs(this->dio);
        this->_cli->setTestTs(this->dio);
    }
};


class detectAnomaliesCommand:public Command {
public:
    detectAnomaliesCommand(DefaultIO* dio, CLI* cli) : Command(dio, cli) {}
    virtual void execute(){
        this->_cli->runDetect(dio);
    }
};

class currentThresholdCommand:public Command{
public:
    currentThresholdCommand(DefaultIO* dio, CLI* cli) : Command(dio, cli) {}
    virtual void execute(){
        this->_cli->setSettings(dio);
    }
};

class displayCommand:public Command{
public:
    displayCommand(DefaultIO* dio, CLI* cli) : Command(dio, cli) {}
    virtual void execute(){
        this->_cli->runDisplay(dio);
    }
};

class analyzeCommand:public Command{
public:
    analyzeCommand(DefaultIO* dio, CLI* cli) : Command(dio, cli) {}
    virtual void execute(){
        this->_cli->runAnalyze(dio);
    }
};

#endif /* COMMANDS_H_ */
