#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    TimeSeries* trainTs;
    TimeSeries* testTs;
    HybridAnomalyDetector* ad;
    vector<AnomalyReport>* r;
}


void printMenu(DefaultIO* dio) {
    string menu = "Welcome to the Anomaly Detection Server.\n"
                  "Please choose an option:\n"
                  "1. upload a time series csv file\n"
                  "2. algorithm settings\n"
                  "3. detect anomalies\n"
                  "4. display results\n"
                  "5. upload anomalies and analyze results\n"
                  "6. exit\n";
    dio->write(menu);
}

void CLI::start(){
    printMenu(this->dio);
    while(true) {
        string index = this->dio->read();
        int i = stoi(index);
        if (6 == i)
            break;
        switch (i) {
            case(1): {
                uploadAtimeSeriesCommand t(dio, trainTs, testTs, ad, r);
                t.execute();
                break;
            }
            case(2): {
                currentThresholdCommand c(dio, trainTs, testTs, ad, r);
                c.execute();
                break;
            }
            case(3): {
                detectAnomaliesCommand d(dio, trainTs, testTs, ad, r);
                d.execute();
                break;
            }
            case(4): {
                displayCommand ds (dio, trainTs, testTs, ad, r);
                ds.execute();
                break;
            }
            case(5): {
                break;
            }
        }
    }

}


CLI::~CLI() {
}

