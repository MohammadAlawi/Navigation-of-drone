/*! @file FlightLibrary.hpp
 *  @version 1.0.0
 *  @date 10.04.2021
 *
 *  @brief
 *  Our brief
*/

#ifndef FLIGHTLIBRARY_H                                 // Guards
#define FLIGHTLIBRARY_H                                 // Guards

// System Includes
#include <cmath>
#include  <signal.h>
#include  <stdlib.h>

// DJI OSDK includes
#include "dji_status.hpp"
#include <dji_vehicle.hpp>
#include <dji_telemetry.hpp>
#include <dji_flight_controller.hpp>

// Helpers
#include <dji_linux_helpers.hpp>

namespace FlightLibrary
{
    class FlightTelemetry
    {
    private:
        /* data */
    public:
        FlightTelemetry();
        ~FlightTelemetry();
        void GetQuaternionData(DJI::OSDK::Vehicle* vehiclePtr);                     // This method gets broadcasted quaternion data and prints it out
        void GetBatteryData(DJI::OSDK::Vehicle* vehiclePtr);                        // This method gets battery data and prints it out
        void GetGlobalPositionData(DJI::OSDK::Vehicle* vehiclePtr);                 // This method gets position data (longtidude, latidude, altidude, height, health)
        void GetLocalPositionData(DJI::OSDK::Vehicle* vehiclePtr);                  // This method gets local position data (X,Y,Z)
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



#endif                                                  // Guards