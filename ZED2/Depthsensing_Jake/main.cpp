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

// Depth sensing example by Jake using ZED V2 camera and ZED OSDK
// Parts of samples from ZED OSDK are utilized


#include <sl/Camera.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sl;

int main(int argc, char **argv) {

    // Create a ZED camera object
    Camera zed;

    // Declare some stuff
    string unit = "M";

    // Set configuration parameters
    // Can be found in API documentation
    InitParameters init_parameters;
    init_parameters.depth_mode = DEPTH_MODE::ULTRA; // Depth mode, available is ULTRA, QUALITY, PERFORMANCE
    init_parameters.coordinate_units = UNIT::METER; // Unit to use (for depth measurements)
    init_parameters.depth_minimum_distance = 0.3 ; // Minimum depth perception, minimum setting for ZED 2 is 0.3m. Increase to increase performance
    // zed.setDepthMaxRangeValue(40); // set maximum depth perception distance to 40m

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != ERROR_CODE::SUCCESS) {
        cout << "Error " << returned_state << ", exit program." << endl;
        return EXIT_FAILURE;
    }

    // Set runtime parameters after opening the camera
    RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = SENSING_MODE::STANDARD; // Use STANDARD sensing mode

    // Capture 50 images and depth, then stop
    int i = 0;

    sl::Mat image, depth, point_cloud;


    // Define a matrix of sectors
    int mat_columns = 128;
    int mat_rows = 72;

    int mat_xoffset, mat_yoffset;
    float maxdistance = 40.0;
    float distance, closestdistance;
    int closest_x, closest_y;

    int x = 0;
    int y = 0;

    while (i < 5000) {

        // A new image is available if grab() returns ERROR_CODE::SUCCESS
        if (zed.grab(runtime_parameters) == ERROR_CODE::SUCCESS) {
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
            cout<<"Shortest distance found at{"<<closest_x<<";"<<closest_y<<"}: "<<closestdistance<<unit<<endl;

            // Increment the loop
            i++;
        }
    }

    // Print image width and height
    cout<<"Image width: "<<image.getWidth()<<endl;
    cout<<"Image height: "<<image.getHeight()<<endl;

    // End message
    cout<<"Exiting program"<<endl;
    // Close the camera
    zed.close();
    return EXIT_SUCCESS;
}
