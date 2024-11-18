#ifndef CPU_TEMPERATURE_H
#define CPU_TEMPERATURE_H

// #####################################################################################
// Include libraries:

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

// ###################################################################################

/**
 * @class CPUTemperature
 */
class CPUTemperature
{
    public:

        /// @brief The last error occurred for the object.
        static std::string errorMessage;

        /// @brief The warning flag that indicates whether the CPU temperature is above or below the critical value.
        static bool warningFlag;

        /// @brief The CPU Temperature sensor value. [celcius]
        static float value;            

        /// @brief Start and open CPU temperature file. Check that cpu temp file can open or not.
        static bool begin(void);

        /// @brief Read and return CPU temperature. [celcius]
        static float read(void);

        static float get(void);

        /// @brief Print last CPU temperature read. [celcius]
        static void print(void);

        /// @brief Stop CPU thread function and close CPU temperature file.
        static void close(void);

        /**
         * @brief Set OS and HYST temperatures used to manage the warning flag state.
         * @param t_os is the temperature value at which the CPUTemperature object sets the warning flag to true.
         * @param t_hyst is the temperature value below which the CPUTemperature object sets the warning flag to false.
         */
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

        /// @brief Open cpu temperature file.
        static void _open(void);

        /// @brief The function reads the CPU temperature value in a thread every 2 seconds.
        static void _readThread(void);
    
};

#endif  // CPU_TEMPERATURE_H

