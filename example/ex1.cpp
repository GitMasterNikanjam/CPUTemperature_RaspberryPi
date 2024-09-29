// For compile: g++ -o ex1 ex1.cpp ../CPUTemperature.cpp  -lbcm2835 -lpthread
// For run: sudo ./ex1
// ######################################################
#include <iostream>
#include "../CPUTemperature.h"
#include <bcm2835.h>
#include <chrono>

using namespace std;

CPUTemperature cpuTemp;

uint64_t t1,t2;         // for time storing.

// Time origin point.
chrono::time_point<std::chrono::system_clock> T_origin;

// Calculate time from origin time point in microseconds.
uint64_t micros(void);

int main(void)
{
    // Save time point at object constructor.
    T_origin = chrono::high_resolution_clock::now();

    cpuTemp.begin();

    for(int i = 0; i <= 1000; i++)
    {
        t1 = micros();
        float temp = cpuTemp.get();
        t2 = micros();
        printf("CPU temperature is: %f. ",temp); // cpuTemp.print();
        printf("read() proccess duration is: %d us\n", t2-t1);
        bcm2835_delay(100);
    }

    cpuTemp.close();

    return 0;
}

uint64_t micros(void)
{
    std::chrono::time_point<std::chrono::system_clock> T_point;
    // Get Current time point
    T_point = std::chrono::high_resolution_clock::now();
    // Calculate Time duration
    auto T_dur = std::chrono::duration_cast<std::chrono::microseconds>(T_point - T_origin);
    // Return time duration in uint64_t 
    return static_cast<uint64_t>(T_dur.count());
}