#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO* dio) {
    TimeSeries* trainTs;
    TimeSeries* testTs;
    HybridAnomalyDetector* ad;
    vector<AnomalyReport>* r;
}


void printMenu() {
    std::cout << "Welcome to the Anomaly Detection Server.\n"
               "Please choose an option:\n"
               "1. upload a time series csv file\n"
               "2. algorithm settings\n"
               "3. detect anomalies\n"
               "4. display results\n"
               "5. upload anomalies and analyze results\n"
               "6. exit" << std::endl;
}

void CLI::start(){
    printMenu();
    int i = 0;
    while(true) {
        std::cin >> i;
        if (6 == i)
            break;
        switch (i) {
            case(1): {
                uploadAtimeSeriesCommand t(trainTs, testTs, ad, r);
                t.execute();
                break;
            }
            case(2): {
                currentThresholdCommand c(trainTs, testTs, ad, r);
                c.execute();
                break;
            }
            case(3): {
                detectAnomaliesCommand d(trainTs, testTs, ad, r);
                d.execute();
                break;
            }
            case(4): {
                displayCommand ds (trainTs, testTs, ad, r);
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

