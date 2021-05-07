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
        << "Altidude "      <<globalposition.altitude<< " "                             // Print out returned data
        << "Heigh "         <<globalposition.height<< " "                               // Print out returned data
        << "Latidude "      <<globalposition.latitude<< " "                             // Print out returned data
        << "Longtidude "    <<globalposition.longitude<< " "                            // Print out returned data
        << "Health "        <<globalposition.health<< " "                               // Print out returned data
        << std::endl;
        sleep(1); 
    }

    ACK::ErrorCode subscribeStatus;
    subscribeStatus = vehicle->subscribe->verify(responseTimeout);
    if (ACK::getError(subscribeStatus) != ACK::SUCCESS)
    {
        ACK::getErrorCodeMessage(subscribeStatus, __func__);                                // Print error message
    }
    // Package 0: Subscribe to flight status at freq 1 Hz
    int pkgIndex                = 0;                                                        // Package index
    int freq                    = 1;                                                        // Refreshing frequency
    TopicName topicList1Hz[]    = { TOPIC_STATUS_FLIGHT };                                  // Topic
    int numTopic                = sizeof(topicList1Hz) / sizeof(topicList1Hz[0]);           // Topic size
    bool enableTimestamp        = false;                                                    // Timestamp use
    bool pkgStatus = vehicle->subscribe->initPackageFromTopicList(pkgIndex, numTopic, topicList1Hz, enableTimestamp, freq);     // Initialization
    if (!(pkgStatus))
    {
        //return pkgStatus;
    }
    subscribeStatus = vehicle->subscribe->startPackage(pkgIndex, responseTimeout);
    if (ACK::getError(subscribeStatus) != ACK::SUCCESS)
    {
        ACK::getErrorCodeMessage(subscribeStatus, __func__);                                // Print error message
        vehicle->subscribe->removePackage(pkgIndex, responseTimeout);                       // Cleanup before return
    }
    TypeMap<TOPIC_STATUS_FLIGHT>::type flightStatus;                                        // Get all the data once before the loop to initialize vars
    flightStatus = vehicle->subscribe->getValue<TOPIC_STATUS_FLIGHT>();                     // Run topic method and return values to struct  






        // Package 1: Subscribe to flight status at freq 1 Hz
           pkgIndex        = 0;
           freq            = 1;
    TopicName topicList50Hz[]  = { TOPIC_ALTITUDE_BAROMETER };
           numTopic        = sizeof(topicList50Hz) / sizeof(topicList50Hz[0]);
          enableTimestamp = false;

     pkgStatus = vehicle->subscribe->initPackageFromTopicList(
        pkgIndex, numTopic, topicList1Hz, enableTimestamp, freq);
    if (!(pkgStatus))
    {
        //return pkgStatus;
    }
    subscribeStatus = vehicle->subscribe->startPackage(pkgIndex, responseTimeout);
    if (ACK::getError(subscribeStatus) != ACK::SUCCESS)
    {
        ACK::getErrorCodeMessage(subscribeStatus, __func__);
        // Cleanup before return
        vehicle->subscribe->removePackage(pkgIndex, responseTimeout);
        //return false;
    }

    TypeMap<TOPIC_ALTITUDE_BAROMETER>::type     barometer;

    flightStatus = vehicle->subscribe->getValue<TOPIC_ALTITUDE_BAROMETER>();




}

void FlightTelemetry::GetLocalPositionData(DJI::OSDK::Vehicle* vehicle)
{
    /*
    std::cout 
    << "X "      <<localpositionv0.x<< " "                                  // Print out returned data
    << "Y "      <<localpositionv0.y<< " "                                  // Print out returned data
    << "Z "      <<localpositionv0.z<< " "                                  // Print out returned data
    << std::endl;
    sleep(1);
    */
   //TypeMap<TOPIC_ALTITUDE_BAROMETER>::type altitude_barometer;

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
