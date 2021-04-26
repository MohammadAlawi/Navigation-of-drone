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

// local includes
#include "placeholder.hpp"

// Using std namespace
using namespace std;
using namespace sl;

const int MAX_CHAR = 128;

int main() {

    Camera zed;
    Pose camera_path;

    sl::float3 translation, rotation;

    POSITIONAL_TRACKING_STATE tracking_state;

    openCamera(zed);

    // Start a supervising thread
    // create a promise and get its future
    promise<bool>p;
    auto future = p.get_future();

    // New thread to supervise
    thread t([&p] {
        userExit();
        p.set_value(true);
        });

    while(future.wait_for(0ms) != std::future_status::ready) {

        // get position
        getPosition(zed, camera_path, translation, rotation);
        cout << translation << endl;
        cout << rotation << endl;

    }
    // join the userexit thread
    t.join();

    // Export the spatial memory for future sessions
    //zed.saveAreaMap("/home/uwb5/git/ZED2/Positionaltracking_Jake/cpp/areafiles/output.area"); // The actual file will be created asynchronously.
    //cout << "Saving area file - "<<zed.getAreaExportState() << endl;
    // the function seems to output success even if no file is created/modified
    // Make sure there is enough tracked data to create file

    // zed.close should already disable this
    zed.disablePositionalTracking();

    //zed.disableRecording();
    zed.close();
    return EXIT_SUCCESS;
}

