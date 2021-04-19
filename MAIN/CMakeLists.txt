# *  @Copyright (c) 2016-2017 DJI
# *
# * Permission is hereby granted, free of charge, to any person obtaining a copy
# * of this software and associated documentation files (the "Software"), to deal
# * in the Software without restriction, including without limitation the rights
# * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# * copies of the Software, and to permit persons to whom the Software is
# * furnished to do so, subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included in
# * all copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# * SOFTWARE.
# *
# *

cmake_minimum_required(VERSION 2.8)
project(MAIN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g -O0 -DNDEBUG")                           # -DNDEBUG added for ZED;

        SET(ZED_DIR "/usr/local/zed/")                                                                  # Added for ZED integration
        FIND_PACKAGE(zed REQUIRED )                                                                     # Added for ZED integration
        INCLUDE_DIRECTORIES(${ZED_INCLUDE_DIRS})                                                        # Added for ZED integration
        link_directories(${ZED_LIBRARY_DIR})                                                            # Added for ZED integration

        find_package(PythonLibs REQUIRED)                                                               # Added for Python integration                
        include_directories(${PYTHON_INCLUDE_DIRS})                                                     # Added for Python integration

# User include directories here
include_directories(${ONBOARDSDK_SOURCE}/api/inc)
include_directories(${ONBOARDSDK_SOURCE}/utility/inc)
include_directories(${ONBOARDSDK_SOURCE}/hal/inc)
include_directories(${ONBOARDSDK_SOURCE}/protocol/inc)
include_directories(${ONBOARDSDK_SOURCE}/platform/linux/inc)
include_directories(${MODULES_HEADER_DIR})
include_directories(${FLIGHT_MODULES_HEADER_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../core/inc)
include_directories(${ZED_CAMERA_INCLUDE_DIR})                                                          # Added for include in ZED install directory
include_directories(${CUDA_INCLUDE_DIR})                                                                # Added for includes in CUDA directory
include_directories(${MAIN_INCLUDE_DIR})                                                                # Added for includes in main directory
include_directories(${PYTHON_H_INCLUDE_DIR})                                                            # Added for includes in Python directory



# User-code related project files - Add here all .hpp and .cpp file directories
FILE(GLOB SOURCE_FILES *.hpp *.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/dji_linux_environment.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/dji_linux_helpers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../core/src/flight_sample.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}MyLibrary.cpp                                                        # Added path to file 
        ${CMAKE_CURRENT_SOURCE_DIR}MyLibrary.hpp                                                        # Added path to file
        ${ZED_CAMERA_LIBRARY_DIR}Camera.hpp                                                             # Added path to file
        ${CUDA_INCLUDE_DIR}cuda.h                                                                       # Added path to file
        ${CUDA_INCLUDE_DIR}cuda_runtime.h                                                               # Added path to file
        ${CUDA_INCLUDE_DIR}cuda_runtime_api.h                                                           # Added path to file
        ${CUDA_INCLUDE_DIR}device_launch_parameters.h                                                   # Added path to file
	${CMAKE_CURRENT_SOURCE_DIR}FlightLibrary.hpp                                                    # Added path to file
	${CMAKE_CURRENT_SOURCE_DIR}FlightLibrary.cpp                                                    # Added path to file
        ${PYTHON_H_INCLUDE_DIR}Python.h                                                                 # Added path to file
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} djiosdk-core)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${ZED_LIBRARIES})                                                 # Added for ZED integration
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})                                              # Added for Python integration


