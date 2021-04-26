/*! @file FlightLibrary.hpp
 *  @version 1.0.0
 *  @date 10.04.2021
 *
 *  @brief
 *  Our brief
*/

#ifndef FLIGHTLIBRARY_H                                 // Guards
#define FLIGHTLIBRARY_H                                 // Guards

// System includes
#include <cmath>                                                                                // Header in standard library
#include <signal.h>                                                                             // Header in standard library
#include <stdlib.h>                                                                             // Header in standard library
#include <csignal>                                                                              // Header in standard library                                                                             
#include <fcntl.h>                                                                              // Header in standard library
#include <iostream>                                                                             // Header in standard library
#include <sys/stat.h>                                                                           // Header in standard library
#include <unistd.h>                                                                             // Header in standard library
#include <string>                                                                               // Header in standard library
#include <string.h>                                                                             // Header in standard library

// Definitions
#define MAX_BUF 1024

// DJI OSDK includes
#include "dji_status.hpp"
#include <dji_vehicle.hpp>
#include <dji_telemetry.hpp>
#include <dji_broadcast.hpp>
#include <dji_subscription.hpp>
#include <dji_flight_controller.hpp>

// Helpers
#include <dji_linux_helpers.hpp>

// Zed includes
#include <Camera.hpp>                                                                         // ZED library inclusion (INSTALLED LIBRARY)

namespace FlightLibrary
{
    class FlightTelemetry
    {
    private:
        /* data */
    public:
        FlightTelemetry();
        ~FlightTelemetry();
        void GetQuaternionData(DJI::OSDK::Vehicle* vehiclePtr);                             // This method gets broadcasted quaternion data and prints it out
        void GetBatteryData(DJI::OSDK::Vehicle* vehiclePtr);                                // This method gets battery data and prints it out
        void GetGlobalPositionData(DJI::OSDK::Vehicle* vehiclePtr, int responseTimeout);    // This method gets position data (longtidude, latidude, altidude, height, health)
        typedef struct UwbStruct                                                            // This attribute is used to store and return UWB data
        {
            float x;                                                                        // This member stores x value
            float y;                                                                        // This member stores y value
            float z;                                                                        // This member stores z value
        }UwbStruct;
        UwbStruct GetUwbPositionData(int fd, char buf[MAX_BUF]);                            // This method gets local position data (X,Y,Z)
        void setTxt(sl::float3 value, char* ptr_txt);                                       // This method gets rotation and translation in text format 
        void openCameraZed(sl::Camera &zed);                                                // ++++ This method initializes Zed camera + Changed function name (DJI have similar name in lib)
        std::pair<sl::float3 , sl::float3> getPositionZed(sl::Camera &zed, sl::Pose &camera_path, sl::float3 &translation, sl::float3 &rotation,  std::pair<sl::float3 , sl::float3> &ReturnPairPosRot);    // ++++ Name changed + Added scopes sl + changed to return type
    };

    class FlightCommander
    {
        private:
        //
        public:
        FlightCommander();
        ~FlightCommander();
        void ForceLanding(DJI::OSDK::Vehicle* vehiclePtr);                          // This method will command vehicle to perform forced landing
    };
    
    
}
void SafetyFunction(int signum);



#endif                                                  // Guards