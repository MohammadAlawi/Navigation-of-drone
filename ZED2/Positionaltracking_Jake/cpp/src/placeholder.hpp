// Functions to enable quick positionaltracking with ZED camera

using namespace std;
using namespace sl;
#include <iostream>

// SL function
void setTxt(sl::float3 value, char* ptr_txt) {
    const int MAX_CHAR = 128;
    snprintf(ptr_txt, MAX_CHAR, "%3.2f; %3.2f; %3.2f", value.x, value.y, value.z);
}

void openCamera(Camera &zed) {
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
        cout << "Camera Open" << returned_state << "Exit program." << endl;
        //return EXIT_FAILURE;
    }

    auto camera_model = zed.getCameraInformation().camera_model;

    // define filepath for area file
    sl::String areafile = "/home/uwb5/git/ZED2/Positionaltracking_Jake/cpp/areafiles/input.area";

    // Set parameters for Positional Tracking
    PositionalTrackingParameters positional_tracking_param;
    positional_tracking_param.enable_area_memory = true;
    //positional_tracking_param.area_file_path = areafile;

    // enable Positional Tracking
    returned_state = zed.enablePositionalTracking(positional_tracking_param);
    if (returned_state != ERROR_CODE::SUCCESS) {
        cout<<"Enabling positionnal tracking failed: "<< returned_state << endl;
        zed.close();
        //return EXIT_FAILURE;
    }
}

void getPosition(Camera &zed, Pose &camera_path) {
    // Pass by reference Camera, Pose

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
                // Get rotation and translation and displays it
                setTxt(camera_path.getEulerAngles(), text_rotation);
                setTxt(camera_path.getTranslation(), text_translation);

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
}

// Supervising function to allow clean exit
void userExit()  {
    int inputchar = 0;
    // ESC = ASCII 27, q = ASCII 113
    while (inputchar != 27 && inputchar != 113) {
        inputchar = getchar();
    }
    cout << "User terminated program" << endl;
    // return

}

// SL function
void parseArgs(int argc, char **argv, sl::InitParameters& param) {
    if (argc > 1 && string(argv[1]).find(".svo") != string::npos) {
        // SVO input mode
        param.input.setFromSVOFile(argv[1]);
        cout << "[Sample] Using SVO File input: " << argv[1] << endl;
    } else if (argc > 1 && string(argv[1]).find(".svo") == string::npos) {
        string arg = string(argv[1]);
        unsigned int a, b, c, d, port;
        if (sscanf(arg.c_str(), "%u.%u.%u.%u:%d", &a, &b, &c, &d, &port) == 5) {
            // Stream input mode - IP + port
            string ip_adress = to_string(a) + "." + to_string(b) + "." + to_string(c) + "." + to_string(d);
            param.input.setFromStream(sl::String(ip_adress.c_str()), port);
            cout << "[Sample] Using Stream input, IP : " << ip_adress << ", port : " << port << endl;
        } else if (sscanf(arg.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d) == 4) {
            // Stream input mode - IP only
            param.input.setFromStream(sl::String(argv[1]));
            cout << "[Sample] Using Stream input, IP : " << argv[1] << endl;
        } else if (arg.find("HD2K") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD2K;
            cout << "[Sample] Using Camera in resolution HD2K" << endl;
        } else if (arg.find("HD1080") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD1080;
            cout << "[Sample] Using Camera in resolution HD1080" << endl;
        } else if (arg.find("HD720") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD720;
            cout << "[Sample] Using Camera in resolution HD720" << endl;
        } else if (arg.find("VGA") != string::npos) {
            param.camera_resolution = sl::RESOLUTION::VGA;
            cout << "[Sample] Using Camera in resolution VGA" << endl;
        }
    } else {
        // Default
    }
}