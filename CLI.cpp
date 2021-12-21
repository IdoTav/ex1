//Ido Tavron 316222512
//Tal Dabran 316040898
#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO *dio) {
    this->dio = dio;
}

/**
* this function prints the menu
*/
void printMenu(DefaultIO *dio) {
    string menu = "Welcome to the Anomaly Detection Server.\n"
                  "Please choose an option:\n"
                  "1.upload a time series csv file\n"
                  "2.algorithm settings\n"
                  "3.detect anomalies\n"
                  "4.display results\n"
                  "5.upload anomalies and analyze results\n"
                  "6.exit\n";
    dio->write(menu);
}

/**
* this function runs the menu and activate the methods by the user's input
*/
void CLI::start() {
    TimeSeries trainTs, testTs;
    HybridAnomalyDetector ad;
    vector<AnomalyReport> ar;
    int numLines;
    while (true) {
        printMenu(this->dio);
        string index = this->dio->read();
        int i = stoi(index);
        if (6 == i)
            break;
        switch (i) {
            case (1): {
                uploadAtimeSeriesCommand ex(dio);
                ex.execute();
                testTs = ex._testTs;
                trainTs = ex._trainTs;
                numLines = ex._numlines;
                break;
            }
            case (2): {
                currentThresholdCommand ex(dio);
                ex.execute();
                ad = ex._ad;
                break;
            }
            case (3): {
                detectAnomaliesCommand ex(dio, &ad, trainTs, testTs);
                ex.execute();
                vector<AnomalyReport> r = ex._ar;
                unsigned long vectorSize = r.size();
                for (int i = 0; i < vectorSize; i++)
                    ar.push_back(r[i]);
                break;
            }
            case (4): {
                displayResult ex(dio, &ar);
                ex.execute();
                break;
            }
            case (5): {
                analyzeresults ex(dio, &ar, numLines);
                ex.execute();
                break;
            }
        }
    }
}

CLI::~CLI() {
}
