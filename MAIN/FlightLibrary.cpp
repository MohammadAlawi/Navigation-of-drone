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
using namespace std;
using namespace sl;

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

    Telemetry::VelocityInfo legacyvelocity;

    for(int i = 0; i <= 10000; i++)
    {
        globalposition = vehicle->broadcast->getGlobalPosition();                       // Run method and return struct type data to globalpostion struct
        legacyvelocity = vehicle->broadcast->getVelocityInfo();
        std::cout 
        << "Altidude "      <<globalposition.altitude<< " "                             // Print out returned data (Barometer)
        << "Heigh "         <<globalposition.height<< " "                               // Print out returned data (Ultrasonic)
        << "Latidude "      <<globalposition.latitude<< " "                             // Print out returned data
        << "Longtidude "    <<globalposition.longitude<< " "                            // Print out returned data
        //<< "Health "        <<globalposition.health<< " "                               // Print out returned data (GPS Health)
        << "Health "        <<(int)legacyvelocity.health<< " "                               // Print out returned data (GPS Health)
        << std::endl;
        sleep(1); 
    }
}

FlightTelemetry::UwbStruct FlightTelemetry::GetUwbPositionData(int fd, char buf[MAX_BUF])
{
    FlightTelemetry::UwbStruct data;
    float ConvertedFloat;
    read(fd, buf, MAX_BUF);
    std::string StringToGetSplitted = buf;
    std::size_t start = StringToGetSplitted.find("X");
    std::size_t end = StringToGetSplitted.find("+");
    std::string SplittedString = StringToGetSplitted.substr(start+1, end-1);
    if(SplittedString.size() == 0)
    {
        std::cout <<"EMPTY STRING"<< std::endl;
        data.x = data.x;                                                    // Use previous string if string is empty
    }
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.x = ConvertedFloat/1000.0;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("Y");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+1, end-1);
    if(SplittedString.size() == 0)
    {
        std::cout <<"EMPTY STRING"<< std::endl;
        data.y = data.y;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.y = ConvertedFloat/1000.0;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("Z");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+1, end-1);
    if(SplittedString.size() == 0)
    {
        std::cout <<"EMPTY STRING"<< std::endl;
        data.z = data.z;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.z = ConvertedFloat/1000.0;                                                                         // Assign values to struct members
    }
    //std::cout <<"X float " << data.x <<" Y float " << data.y <<" Z float " << data.z << std::endl;        // Print data (Optional)
    return data;                                                                                            // Return struct
}

void FlightTelemetry::setTxt(sl::float3 value, char* ptr_txt) {
    const int MAX_CHAR = 128;
    snprintf(ptr_txt, MAX_CHAR, "%3.2f; %3.2f; %3.2f", value.x, value.y, value.z);
}

void FlightTelemetry::openCameraZed(sl::Camera &zed)                                                        // ++++ Changed scope to st::Camera
{
    
    // Set configuration parameters for the ZED
    InitParameters init_parameters;
    init_parameters.coordinate_units = UNIT::METER;
    init_parameters.coordinate_system = COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP;
    init_parameters.sdk_verbose = true;

    string unit = "M";

    init_parameters.depth_mode = DEPTH_MODE::PERFORMANCE; // Depth mode, available is ULTRA, QUALITY, PERFORMANCE (quality somehow has the highest execution time)
    init_parameters.coordinate_units = UNIT::METER; // Unit to use (for depth measurements)
    init_parameters.depth_minimum_distance = 0.3 ; // Minimum depth perception, minimum setting for ZED 2 is 0.3m. Increase to increase performance
    // zed.setDepthMaxRangeValue(40); // set maximum depth perception distance to 40m

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != ERROR_CODE::SUCCESS) {
        cout << "Camera Open " << returned_state << " Exit program." << endl;
        //return EXIT_FAILURE;
    }
    else 
    {
        std::cout << "Zed camera opened initialization status " << returned_state << std::endl;             // ++++ Added print to check the status
    }

    auto camera_model = zed.getCameraInformation().camera_model;

    // define filepath for area file
    sl::String areafile = "/home/uwb5/git/ZED2/Positionaltracking_Jake/cpp/areafiles/input.area";           // ++++ Check this path

    // Set parameters for Positional Tracking
    PositionalTrackingParameters positional_tracking_param;
    positional_tracking_param.enable_area_memory = true;
    //positional_tracking_param.area_file_path = areafile;

    // enable Positional Tracking
    returned_state = zed.enablePositionalTracking(positional_tracking_param);
    if (returned_state != ERROR_CODE::SUCCESS) {
        cout<<"Enabling positional tracking failed: "<< returned_state << endl;
        zed.close();
        //return EXIT_FAILURE;
    }
    
}
std::pair<sl::float3 , sl::float3> FlightTelemetry::getPositionZed(sl::Camera &zed, sl::Pose &camera_path, sl::float3 &translation, sl::float3 &rotation, std::pair<sl::float3 , sl::float3> &ReturnPairPosRot) {   // ++++ Added scopes sl
    // Pass by reference Camera, Pose, Translation, rotation

    const int MAX_CHAR = 128;

    POSITIONAL_TRACKING_STATE tracking_state;
    Timestamp lasttimestamp;

    // Create text for GUI
    char text_rotation[MAX_CHAR];
    char text_translation[MAX_CHAR];

    if (zed.grab() == ERROR_CODE::SUCCESS) {
            // Get the position of the camera in a fixed reference frame (the World Frame)
            // Get timestamp of last grab
            lasttimestamp = camera_path.timestamp;
            tracking_state = zed.getPosition(camera_path, REFERENCE_FRAME::WORLD);

            if (tracking_state == POSITIONAL_TRACKING_STATE::OK) {
                // Get rotation and translation
                rotation = camera_path.getEulerAngles();
                translation = camera_path.getTranslation();
                
                ReturnPairPosRot.first = translation;
                ReturnPairPosRot.second = rotation;
                
                //std::cout << "Element 0 from rotation " << ReturnPairPosRot.first[0] << "Element 3 from rotation " << ReturnPairPosRot.second[0] << std::endl;

                // Get rotation and translation in text format
                setTxt(rotation, text_rotation);
                setTxt(translation, text_translation);

                // Print to terminal
                // Timestamp is only since last image, not since last successful position
                cout << "Position updated [";
                cout << camera_path.timestamp.getMilliseconds() - lasttimestamp.getMilliseconds();
                cout << " ms] "<< "Confidence: ["<<camera_path.pose_confidence<<"]"<<endl;
                cout << "translation: "<< text_translation << endl;
                cout << "rotation "<< text_rotation << endl;
            }
            else {
                // if tracking state is not ok, print status
                cout << "No position found"<<endl;
                cout << "Tracking state: "<<tracking_state<<endl;
            }
    }
    else {
        sleep_ms(1);
    }

    return ReturnPairPosRot;
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
