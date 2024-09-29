#ifndef CPU_TEMPERATURE_H
#define CPU_TEMPERATURE_H

// ######################################
// Include libraries:

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

// ######################################

std::string getCPUtemperature();

// CPUTemperature object
class CPUTemperature
{
    public:

        // Store last error accured.
        static std::string errorMessage;

        static bool warning;

        // CPU Temperature sensor value. [celcius]
        static float value;            

        // Start and open CPU temperature file. Check that cpu temp file can open or not.
        static bool begin(void);

        // Read and return CPU temperature. [celcius]
        static float read(void);

        static float get(void);

        // Print last CPU temperature read. [celcius]
        static void print(void);

        // Stop and close cpu temperature file.
        static void close(void);

        static void setTempWarning(float t_os, float t_hyst);

    private:

        // ifstream file for temperature file.
        static std::ifstream tempFile;
        static std::string temperatureStr;
        static std::thread temp_thread;
        static std::mutex mtx;
        static bool stopThread;

        static float tempOS;                // Temperature value that over it CPUTemperature object turn on warning flag.
        static float tempHYST;              // Temperature value that bellow it CPUTemperature object turn off warning flag.

        // Open cpu temperature file.
        static void open(void);

        static void _readThread(void);
    
};

#endif  // CPU_TEMPERATURE_H

