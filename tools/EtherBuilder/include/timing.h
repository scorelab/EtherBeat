/*
 * Created by prabushitha on 7/1/18.
*/
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <string>

class TimeCalculator {
public:
    long int timeStart;
    long int getCurrentTimeMilli(){
        auto ms = std::chrono::duration_cast< std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        );

        return ms.count();
    };

    void setStart(){
        timeStart = getCurrentTimeMilli();
    };



    void printElapsedTime() {
        long int timeEnd = getCurrentTimeMilli();
        std::cout << "Time taken = " << timeEnd-timeStart << " ms" << std::endl;
    }
};
