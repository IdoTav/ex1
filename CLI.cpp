#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
}

void printMenu() {
    std::cout<<"Welcome to the Anomaly Detection Server.\n"
               "Please choose an option:\n"
               "1. upload a time series csv file\n"
               "2. algorithm settings\n"
               "3. detect anomalies\n"
               "4. display results\n"
               "5. upload anomalies and analyze results\n"
               "6. exit"<<std::endl;
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
                //UPLOAD A TIME SERIES CSV FILE
            }

            case(2): {
                //ALGORITHM SETTINGS
            }
            case(3): {
                //DETECT ANOMALIES
            }
            case(4): {
                //DISPLAY RESULTS
            }
            case(4): {
                //UPLOAD ANOMALIES AND ANALYZE RESULTS
            }
        }
    }

}


CLI::~CLI() {
}

