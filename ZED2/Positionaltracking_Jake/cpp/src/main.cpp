///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2021, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
Uses part of a ZED provided code for positional tracking
 **************************************************************************/

// ZED includes
#include <sl/Camera.hpp>

// other includes
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

// local includes
#include "includetest.hpp"

// Using std namespace
using namespace std;
using namespace sl;

#define IMU_ONLY 0
const int MAX_CHAR = 128;

inline void setTxt(sl::float3 value, char* ptr_txt) {
    snprintf(ptr_txt, MAX_CHAR, "%3.2f; %3.2f; %3.2f", value.x, value.y, value.z);
}

void parseArgs(int argc, char **argv, sl::InitParameters& param);

void userExit();


int main(int argc, char **argv) {

    Camera zed;
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

    parseArgs(argc, argv, init_parameters);

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != ERROR_CODE::SUCCESS) {
        cout << "Camera Open" << returned_state << "Exit program." << endl;
        return EXIT_FAILURE;
    }

    auto camera_model = zed.getCameraInformation().camera_model;

    // Create text for GUI
    char text_rotation[MAX_CHAR];
    char text_translation[MAX_CHAR];

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
        return EXIT_FAILURE;
    }

    Pose camera_path;
    POSITIONAL_TRACKING_STATE tracking_state;

    sl::Timestamp lasttimestamp;

    // testing depth sensing simultaneously
    sl::Mat image, depth, point_cloud;


    // Define a matrix of sectors seems to perform ok
    // 640x360 maximum
    // 128x72 works
    int mat_columns = 128;
    int mat_rows = 72;

    int mat_xoffset, mat_yoffset;
    float maxdistance = 40.0;
    float distance, closestdistance;
    int closest_x, closest_y;

    int x = 0;
    int y = 0;

    int sector_j, sector_k;

    // Create or open textfile
    ofstream write("depthdetections.txt", ios::app);
    if (!write) {
        cout << "Error Opening File" << endl;
        return -1;
        }

    string currentline;
    string startmessage = "----- Logging started -----";

    write << startmessage << endl;


#if IMU_ONLY
    SensorsData sensors_data;
#endif

    // create a promise and get its future
    promise<bool>p;
    auto future = p.get_future();

    // New thread to supervise
    thread t([&p] {
        userExit();
        p.set_value(true);
        });

    while(future.wait_for(0ms) != std::future_status::ready) {

        if (zed.grab() == ERROR_CODE::SUCCESS) {
            // Get the position of the camera in a fixed reference frame (the World Frame)
            // Get timestamp of last grab
            lasttimestamp = camera_path.timestamp;
            tracking_state = zed.getPosition(camera_path, REFERENCE_FRAME::WORLD);

#if IMU_ONLY
            if (zed.getSensorsData(sensors_data, TIME_REFERENCE::IMAGE) == sl::ERROR_CODE::SUCCESS) {
                setTxt(sensors_data.imu.pose.getEulerAngles(), text_rotation); //only rotation is computed for IMU
                //viewer.updateData(sensors_data.imu.pose, string(text_translation), string(text_rotation), sl::POSITIONAL_TRACKING_STATE::OK);
            }
#else
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

            // Depth sensing

            // Retrieve left image
            zed.retrieveImage(image, VIEW::LEFT);
            // Retrieve depth map. Depth is aligned on the left image
            zed.retrieveMeasure(depth, MEASURE::DEPTH);
            // Retrieve colored point cloud. Point cloud is aligned on the left image.
            zed.retrieveMeasure(point_cloud, MEASURE::XYZRGBA);

            // Get and print distance value
            // We measure the distance camera - object using Euclidean distance
            // Note: x and y coordinates are in pixels, with 0,0 at top left corner

            // Decide offsets for each sector
            mat_xoffset = image.getWidth() / mat_columns;
            mat_yoffset = image.getHeight() / mat_rows;

            // set start sector center
            x = mat_xoffset / 2;
            y = mat_yoffset / 2;

            sl::float4 point_cloud_value;

            // Reset shortest distance
            closestdistance = maxdistance;
            int j = 0;
            int k = 0;

            // Go through the columns and rows
            while (j < mat_rows) {
                while (k < mat_columns) {
                    // Get current value
                    point_cloud.getValue(x, y, &point_cloud_value);

                    // If it is valid, calculate distance
                    if(std::isfinite(point_cloud_value.z)){
                        distance = sqrt(point_cloud_value.x * point_cloud_value.x + point_cloud_value.y * point_cloud_value.y + point_cloud_value.z * point_cloud_value.z);
                        //cout<<"Distance to Camera at position {"<<x<<";"<<y<<"}: "<<distance<<unit<<endl;

                        // IF valid and closer than shortestdistance, save dist and pos
                        if (distance < closestdistance) {
                        closestdistance = distance;
                        closest_x = x;
                        closest_y = y;
                        sector_j = j;
                        sector_k = k;
                        // print every shortest distance found to debug
                        //cout<<"new shortest distance found at {"<<x<<";"<<y<<"}: "<<distance<<unit<<endl;
                        }

                    }else {
                        //cout<<"The Distance can not be computed at {"<<x<<";"<<y<<"}"<<endl;
                        distance = maxdistance;
                        }

                    // Sector scanned, change sector
                    //cout<<"k iteration {"<<k<<"}"<<" at "<<x<<";"<<y<<endl;
                    x = x + mat_xoffset;
                    k++;
                    }

                    //cout<<"j iteration {"<<j<<"}"<<" at "<<x<<";"<<y<<endl;
                    // Row scanned, change row
                    x = mat_xoffset / 2;
                    k = 0;
                    y = y + mat_yoffset;
                    j++;
                }

            // Print results
            //cout<<"Shortest distance found at{"<<closest_x<<";"<<closest_y<<"}: "<<closestdistance<<unit<<endl;
            //write<<"Shortest distance found at{"<<closest_x<<";"<<closest_y<<"}: "<<closestdistance<<unit<<endl;


#endif

        } else  {
            sleep_ms(1);
        }
    }
    // join the userexit thread
    t.join();

    // Export the spatial memory for future sessions
    zed.saveAreaMap("/home/uwb5/git/ZED2/Positionaltracking_Jake/cpp/areafiles/output.area"); // The actual file will be created asynchronously.
    cout << "Saving area file - "<<zed.getAreaExportState() << endl;
    // the function seems to output success even if no file is created/modified
    // Make sure there is enough tracked data to create file

    // Close file
    string endmessage = "----- Logging ended -----";
    write << endmessage << endl;
    write.close();

    // zed.close should already disable this
    zed.disablePositionalTracking();

    //zed.disableRecording();
    zed.close();
    return EXIT_SUCCESS;
}

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


#include <iostream>

using namespace std;

void userExit()  {
    int inputchar = 0;
    // ESC = ASCII 27, q = ASCII 113
    while (inputchar != 27 && inputchar != 113) {
        inputchar = getchar();
    }
    cout << "User terminated program" << endl;
    // return

}

