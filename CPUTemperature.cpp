#include "CPUTemperature.h"

// Define static member variables
std::string CPUTemperature::errorMessage;
bool CPUTemperature::warningFlag = false; 
float CPUTemperature::value = 0.0f;
std::ifstream CPUTemperature::tempFile;
std::string CPUTemperature::temperatureStr;
std::thread CPUTemperature::temp_thread;
std::mutex CPUTemperature::mtx;
bool CPUTemperature::stopThread = false;
float CPUTemperature::tempOS = 80.0;                
float CPUTemperature::tempHYST = 75.0;

bool CPUTemperature::begin(void)
{
    _open();
    
    // Just test tempFile open or not.
    if (!tempFile.is_open()) 
    {
        errorMessage = "CPUTemperature Can not begin. Error opening temperature file";
        // std::cerr << "Error opening temperature file" << std::endl;
        return false;
    }

    stopThread = false; 

    // Create a thread to read temperature
    temp_thread = std::thread(_readThread);

    return true;
}

float CPUTemperature::read(void)
{
    std::lock_guard<std::mutex> lock(mtx);  // Lock to ensure thread safety

    // if temFile not opened, then return 0. 
    if (!tempFile.is_open()) 
    {
        errorMessage = "Error: temperature file is not open. Are you sure do begin() before?";
        return 0;
    }
    
    tempFile.clear();  // Clear any error flags
    tempFile.seekg(0); // Rewind to the beginning of the file

    if (!(tempFile >> temperatureStr))
    {
        errorMessage = "Error reading temperature data";
        return 0.0f;
    }

    try
    {
        // The temperature is in millidegrees Celsius, so divide by 1000 to get degrees Celsius
        value = std::stoi(temperatureStr) / 1000.0f;
    }
    catch (const std::exception& e)
    {
        errorMessage = "Error parsing temperature data";
        return 0.0f;
    }

    if( (value > tempOS) && (warningFlag == false) )
    {
        warningFlag = true;
    }
    else if((value < tempHYST) && (warningFlag == true))
    {
        warningFlag = false;
    }

    return value;
}

void CPUTemperature::setTempWarning(float t_os, float t_hyst)
{
    tempOS = t_os;
    tempHYST = t_hyst;
}

void CPUTemperature::_readThread(void)
{
    while (!stopThread)
    {
        read();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Adjust the sleep duration as needed
    }
}

float CPUTemperature::get(void)
{
    //std::lock_guard<std::mutex> lock(mtx);  // Lock to ensure thread safety
    return value;
}

void CPUTemperature::print(void)
{
    std::cout << value << "[Celcius]" << std::endl; 
}

void CPUTemperature::close(void)
{
    stopThread = true;
    if (temp_thread.joinable())
    {
        temp_thread.join(); // Wait for the thread to finish
    }
    if (tempFile.is_open())
    {
        tempFile.close();
    }
}

void CPUTemperature::_open(void)
{
    tempFile.open("/sys/class/thermal/thermal_zone0/temp");
}