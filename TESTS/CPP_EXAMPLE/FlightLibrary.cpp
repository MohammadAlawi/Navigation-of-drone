/*! @file FlightLibrary.cpp
 *  @version 1.0.0
 *  @date 10.04.2021
 *
 *  @brief
 *  Our brief
*/

#include "FlightLibrary.hpp"                        // FlightLibrary header

using namespace FlightLibrary;                      // Namespace for FlightLibrary
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

/*
   template <TopicName TOPIC_ALTITUDE_BAROMETER>
struct TypeMap
{
  typedef void type;
};

TypeMap<TOPIC_ALTITUDE_BAROMETER>::type altitude_barometer;
*/


//*************************************************************************************************************************************************************************
// FlightTelemetry definitions

FlightTelemetry::FlightTelemetry()                  // Constructor
{
    //
}

FlightTelemetry::~FlightTelemetry()                 // Destructor
{
    //
}

void FlightTelemetry::GetQuaternionData(DJI::OSDK::Vehicle* vehicle)
{
    Telemetry::Quaternion quaternion;                               // Instantiate typedef struct
    for(int i = 0; i <= 100; i++)
    {
        quaternion = vehicle->broadcast->getQuaternion();           // Run method and return struct type data to quaternion struct
        std::cout << "Attitude Quaternion (w,x,y,z) W " <<quaternion.q0<< " X " <<quaternion.q1<< " Y " <<quaternion.q2<< " Z " <<quaternion.q3<< std::endl;   // Print out returned data
        sleep(1);
    }
}

void FlightTelemetry::GetBatteryData(DJI::OSDK::Vehicle* vehicle)
{
    Telemetry::Battery battery;                                                     // Instantiate typedef struct
    battery = vehicle->broadcast->getBatteryInfo();                                 // Run method and return struct type data to battery struct
    std::cout << "Battery data " <<battery.voltage<< std::endl;                     // Print out returned data
}

void FlightTelemetry::GetGlobalPositionData(DJI::OSDK::Vehicle* vehicle, int responseTimeout)
{
    Telemetry::GlobalPosition globalposition;                                           // Instantiate typedef struct
    for(int i = 0; i <= 100; i++)
    {
        globalposition = vehicle->broadcast->getGlobalPosition();                       // Run method and return struct type data to globalpostion struct
        std::cout 
        << "Altidude "      <<globalposition.altitude<< " "                             // Print out returned data (Barometer)
        << "Heigh "         <<globalposition.height<< " "                               // Print out returned data (Ultrasonic)
        << "Latidude "      <<globalposition.latitude<< " "                             // Print out returned data
        << "Longtidude "    <<globalposition.longitude<< " "                            // Print out returned data
        << "Health "        <<globalposition.health<< " "                               // Print out returned data (GPS Health)
        << std::endl;
        sleep(1); 
    }
}

FlightTelemetry::UwbStruct FlightTelemetry::GetUwbPositionData(int fd, char buf[MAX_BUF])
{
    FlightTelemetry::UwbStruct data;
    read(fd, buf, MAX_BUF);
    std::string StringToGetSplitted = buf;
    std::size_t start = StringToGetSplitted.find("X");
    std::size_t end = StringToGetSplitted.find("+");
    std::string SplittedString = StringToGetSplitted.substr(start+1, end-1);
    float ConvertedFloat = std::stof(SplittedString);
    data.x = ConvertedFloat;                                                                                // Assign values to struct members

    start = StringToGetSplitted.find("Y");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+1, end-1);
    ConvertedFloat = std::stof(SplittedString);
    data.y = ConvertedFloat;                                                                                // Assign values to struct members

    start = StringToGetSplitted.find("Y");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+1, end-1);
    ConvertedFloat = std::stof(SplittedString);
    data.z = ConvertedFloat;                                                                                // Assign values to struct members
    //std::cout <<"X float " << data.x <<" Y float " << data.y <<" Z float " << data.z << std::endl;        // Print data (Optional)
    return data;                                                                                            // Return struct
}

//*************************************************************************************************************************************************************************
// FlightCommander definitions

FlightCommander::FlightCommander()
{
    //
}

FlightCommander::~FlightCommander()
{
    //
}

void FlightCommander::ForceLanding(DJI::OSDK::Vehicle* vehicle)
{
    std::cout << "Landing commanded" << std::endl;
    int timeout = 1;
    for(int i = 0; i <= 1; i++)
    {
        vehicle->control->land(timeout);
        sleep(1);
    }
    std::cout << "Landing shall start" << std::endl;
}

void SafetyFunction(int signum)
{
    std::cout << std::endl << "Safety function called. Interrupt # " <<signum<< std::endl;
    exit(signum);
}
