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

FlightTelemetry::UwbStruct FlightTelemetry::GetUwbPositionData(int fd, char buf[MAX_BUF], double lastPosX, double lastPosY)
{
    FlightTelemetry::UwbStruct data {0};
    
    float ConvertedFloat;
    read(fd, buf, MAX_BUF);
    std::string StringToGetSplitted = buf;
    std::size_t start = StringToGetSplitted.find("pX");
    std::size_t end = StringToGetSplitted.find("+");
    std::string SplittedString = StringToGetSplitted.substr(start+2, end-2);
    //std::cout << "String: " << SplittedString << std::endl;
    if(SplittedString.size() == 0 || SplittedString == "-")
    {
        std::cout <<"EMPTY or FAULTY string"<< std::endl;
        data.pX = data.pX;                                                    // Use previous string if string is empty
    }
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.pX = ConvertedFloat/1000.0;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("pY");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+2, end-2);
    //std::cout << "String: " << SplittedString << std::endl;
    if(SplittedString.size() == 0 || SplittedString == "-")
    {
        std::cout <<"EMPTY or FAULTY string"<< std::endl;
        data.pY = data.pY;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.pY = ConvertedFloat/1000.0;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("pZ");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+2, end-2);
    //std::cout << "String: " << SplittedString << std::endl;
    if(SplittedString.size() == 0 || SplittedString == "-")
    {
        std::cout <<"EMPTY or FAULTY string"<< std::endl;
        data.pZ = data.pZ;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.pZ = ConvertedFloat/1000.0;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("aX");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+2, end-4);
    //std::cout << "String: " << SplittedString << std::endl;
    if(SplittedString.size() == 0 || SplittedString == "-")
    {
        std::cout <<"EMPTY or FAULTY string"<< std::endl;
        data.aX = data.aX;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.aX = ConvertedFloat;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("aY");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+2, end-4);
    //std::cout << "String: " << SplittedString << std::endl;
    if(SplittedString.size() == 0 || SplittedString == "-")
    {
        std::cout <<"EMPTY or FAULTY string"<< std::endl;
        data.aY = data.aY;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.aY = ConvertedFloat;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("aZ");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+2, end-4);
    //std::cout << "String: " << SplittedString << std::endl;
    if(SplittedString.size() == 0 || SplittedString == "-")
    {
        std::cout <<"EMPTY or FAULTY string"<< std::endl;
        data.aZ = data.aZ;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.aZ = ConvertedFloat;                                                                         // Assign values to struct members
    }

    start = StringToGetSplitted.find("eH");
    end = StringToGetSplitted.find("+");
    SplittedString = StringToGetSplitted.substr(start+2, end-2);
    //std::cout << "String: " << SplittedString << std::endl;
    if(SplittedString.size() == 0 || SplittedString == "-")
    {
        std::cout <<"EMPTY or FAULTY string"<< std::endl;
        data.eH = data.eH;                                                    // Use previous string if string is empty
    }    
    else
    {
        ConvertedFloat = std::stof(SplittedString);
        data.eH = ConvertedFloat;                                                                         // Assign values to struct members
    }                            

    /*
    std::cout   <<"pX float "  << data.pX <<" pY float " << data.pY <<" pZ float " << data.pZ 
                <<" aX float " << data.aX <<" aY float "  << data.aY <<" aZ float " << data.aZ
                <<" eH float " << data.eH
    << std::endl;                                                                                           // Print data (Optional)
    */
    //************* EXTENDED KALMAN FILTER IMPLEMENTATION START *************
    /*
    // Comment this part out if EKF is not used
    double A_k [2][2] = {{1.0, 0.0}, {0.0, 1.0}};    // A matrix, express how the states x and y changes
    double V_k [2] = {0.01, 0.01};                   // Noise applied to the forward kinematics
    double Q_k [2][2] = {{1.0, 0}, {0, 1.0}};        // State model noise covariance
    double H_k [2][2] = {{1.0, 0}, {0, 1.0}};        // Convert the predicted sensor measurements at time k
    double R_k [2][2] = {{1.0, 0}, {0, 1.0}};        // Sensor measurement noise covariance
    double w_k [2] = {0.07, 0.07};                   // Sensor measurement
    double u_k [2] = {4.5, 0};                       // Control vector
    double X_k [2] = {0, 0};                         // State estimate
    double P_k [2][2] = {{0.01, 0}, {0, 0.01}};      // Initial error covariance
    double Y_k;                                      // Measurement residual
    double S_k;                                      // Measurement residual covariance
    double K_k;                                      // Initial Kalman gain
    double Z_k [2]= {lastPosX, lastPosY};            // Initial Kalman gain 

    double testMatrice [3][2] = {{1, 2}, {3, 4} , {5, 6}};
    double test3DMatrice [4][3][2] = {{{1, 2}, {3, 4} , {5, 6}}, {{7, 8}, {9, 10} , {11, 12}}, {{7, 8}, {9, 10} , {11, 12}}, {{7, 8}, {9, 10} , {11, 12}}};
/*
    int product[10][10], r1=3, c1=3, r2=3, c2=3, i, j, k;
    int a[3][3] = { {2, 4, 1} , {2, 3, 9} , {3, 1, 8} };
    int b[3][3] = { {1, 2, 3} , {3, 6, 1} , {2, 4, 7} };
    if (c1 != r2) {
        cout<<"Column of first matrix should be equal to row of second matrix";
    } else {
        cout<<"The first matrix is:"<<endl;
        for(i=0; i<r1; ++i) {
            for(j=0; j<c1; ++j)
            cout<<a[i][j]<<" ";
            cout<<endl;
        }
        cout<<endl;
        cout<<"The second matrix is:"<<endl;
        for(i=0; i<r2; ++i) {
            for(j=0; j<c2; ++j)
            cout<<b[i][j]<<" ";
            cout<<endl;
        }
        cout<<endl;
        for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j) {
            product[i][j] = 0;
        }
        for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j)
        for(k=0; k<c1; ++k) {
            product[i][j]+=a[i][k]*b[k][j];
        }
        cout<<"Product of the two matrices is:"<<endl;
        for(i=0; i<r1; ++i) {
            for(j=0; j<c2; ++j)
            cout<<product[i][j]<<" ";
            cout<<endl;
        }
    }
    */
   /*


    int i = 0;
    int j = 0;
    int arrRow = *(&test3DMatrice + 1) - test3DMatrice;
    int arrSize = sizeof(test3DMatrice[0][0]) / sizeof(test3DMatrice[0][0][0]);
    int arrColumn = sizeof(test3DMatrice);
    std::cout << "Size of Matrix " << arrSize << std::endl;
    std::cout << "Size of Matrix " << arrColumn/3 << std::endl;
    std::cout << "Size of Matrix " << (arrColumn/3)/8 << std::endl;
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 2; j++)
        {
            std::cout << testMatrice[i][j] << " ";
        }
        std::cout << "" << std::endl
    }
    */
    /*
    
    // State estimate
    X_k = (A_k * X_k) + u_k + V_k;


    // Covariance of the state
    P_k = (A_k * P_k * A_k) + Q_k;

    //#######  measurement (update) ########

    // Y_k residual measurement
    Y_k =  Z_k - ((H_k * X_k) + w_k);

    // Residual covariance
    S_k = (H_k * P_k * H_k) + R_k;

    // Kalman gain
    K_x = (P_k * H_k) / S_k;

    // Updated state estimate
    X_k = X_k + (K_k * Y_k);

    // Updated covariance od the state
    P_x = P_k - (K_k * H_k * P_k);

    // Returns updated value for Pos_x and Pos_y
    data.pX = X_k[0];
    data.aY = X_k[1];
    //reurtn [X_k[0], X_k[1]];    

    //************* EXTENDED KALMAN FILTER IMPLEMENTATION END *************
*/
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
                /*
                cout << "Position updated [";
                cout << camera_path.timestamp.getMilliseconds() - lasttimestamp.getMilliseconds();
                cout << " ms] "<< "Confidence: ["<<camera_path.pose_confidence<<"]"<<endl;
                cout << "translation: "<< text_translation << endl;
                cout << "rotation "<< text_rotation << endl;
                */
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
