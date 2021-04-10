/*! @file MAIN/main.cpp
 *  @version 1.0.0
 *  @date 10.04.2021
 *
 *  @brief
 *  Our brief
*/

#include "flight_control_sample.hpp"
#include "flight_sample.hpp"
#include "MyLibrary.hpp"                                                                      // Local include
#include <Camera.hpp>                                                                         // ZED library inclusion (INSTALLED LIBRARY)

using namespace sl;                                                                           // Namespace definition for ZED library 
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;
using namespace MyNameSpaceFirst;                                                             // Namespace definition (OPTIONAL)
using namespace MyNameSpaceFirst::MyNameSpaceSecond;                                          // Namespace definition (OPTIONAL)

MyClassOne::MyStructOne InstanceStructOne;                                                    // Create instance of struct (utilizing namespace)
MyClassOne::MyEnumOne myenumone;                                                              // Create instance of enum (utilizing namespace)
MyControlClass::ControlStruct controlstructmain;                                              // Create instance of struct (utilizing namespace)
MyControlClass::ControlEnum controlenummain;                                                  // Create instance of enum (utilizing namespace)

int main(int argc, char** argv) {
  
  //*********************************************************************************************************************************************
  // ZED integration
  
  sl::Camera zed;                                                                             // Create instance(object) of class from the ZED library
  
  ERROR_CODE returned_state = zed.open();                                                     // Open the camera
  if (returned_state != ERROR_CODE::SUCCESS) {
      //std::cout << "Error " << returned_state << ", exit program.\n";
      //return EXIT_FAILURE;
      std::cout << "Error " << returned_state << ", No camera found.\n";                      // Added test comment
  }
  auto camera_infos = zed.getCameraInformation();                                             // Get camera information (ZED serial number)
  printf("Hello! This is my serial number: %d\n", camera_infos.serial_number);
  zed.close();                                                                                // Close the camera
  
  //*********************************************************************************************************************************************
  // POZYX integration

  //*********************************************************************************************************************************************
  // MyLibrary integration

  MyClassOne MyInstanceClassOne;                                                              // Create instance(object) of class from the library
  MyControlClass::ClassInMyControlClass MyInstanceClassInControlClass;                        // Create instance(object) of class from the library
  MyInstanceClassOne.PrintFromFirstClass();                                                   // Call function of class from the library
  MyInstanceClassInControlClass.FunctionInClassInMyControlClass();                            // Call function of class of class from the library
  InstanceStructOne.MyIntInStructOne = 88;                                                    // Overwrite value for member of struct in class
  std::cout << "MyIntInStructOne = " <<InstanceStructOne.MyIntInStructOne<< std::endl;        // Access to struct
  std::cout << "MyIntInStructTwo = " <<InstanceStructOne.MyIntInStructTwo<< std::endl;        // Access to struct
  std::cout << "MyIntInStructThree = " <<InstanceStructOne.MyIntInStructThree<< std::endl;    // Access to struct
  std::cout << "MyIntInStructFour = " <<InstanceStructOne.MyIntInStructFour<< std::endl;      // Access to struct
  std::cout << "MyIntInStructFive = " <<InstanceStructOne.MyIntInStructFive<< std::endl;      // Access to struct
  std::cout << "MyIntInStructSix = " <<InstanceStructOne.MyIntInStructSix<< std::endl;        // Access to struct
  std::cout << "MyEnum = " <<MyClassOne::MyEnumOne::MyEnumMemberOne<< std::endl;              // Access to enum (CAN BE DONE DIRECTLY)

  MyControlClass MyInstanceControlClass;                                                                                                          // Create instance of class from library
  int VariableToBePointedAt = 9;                                                                                                                  // Create variable to be passed to function
  MyInstanceControlClass.FunctionThatTakesPointerVariableAndStructAndEnumAsParameter(&VariableToBePointedAt, controlstructmain, controlenummain); // Call function

  MyClassTwo MyInstanceClassTwo;                                                              // Create instance(object) of class

  //MyInstanceClassInControlClass.MyTemplateFunction<int>(x, y);                              // UNDER WORK
  
  MyClassTwo::MyVector VectorInstanceToBePassed;                                              // Create instance of vector using typedef from library
  MyInstanceClassTwo.PrintVectorCalculations(VectorInstanceToBePassed);                       // Call function that takes vector as parameter
  MyClassTwo::MyArray ArrayInstanceToBePassed;                                                // Create instance of array using typedef from library
  MyInstanceClassTwo.PrintArrayCalculations(ArrayInstanceToBePassed);                         // Call function that takes array as parameter
  std::cout << "MyIntegerInMyClassTwo in MyClassTwo = " <<MyInstanceClassTwo.MyIntegerInMyClassTwo<< std::endl;      // Print integer from library

  ClassUtilizingPointers* classutilizingpointers;                                             // Create instance of class pointer and point to class
  classutilizingpointers->FunctionInClassUtilizingPointers();                                 // Call method(function) of class using pointers

  ClassUtilizingPointers::ClassInClassUtilizingPointers* classinclassutilizingpointers;       // Create instance of class pointer and point to class
  classinclassutilizingpointers->FunctionInClassInClassUtilizingPointers();                   // Call method(function) of class using pointers

  classutilizingpointers->myclasstwo->PrintVectorCalculations(VectorInstanceToBePassed);      // Call method(function) of class using pointer from one class to other class
  classutilizingpointers->myclassone->PrintFromFirstClass();                                  // Call method(function) of class using pointer from one class to other class
  
  
  //*********************************************************************************************************************************************
  // OSDK integration
  
  // Initialize variables
  int functionTimeout = 1;
  // Setup OSDK.
  LinuxSetup linuxEnvironment(argc, argv);
  Vehicle* vehicle = linuxEnvironment.getVehicle();
  if (vehicle == NULL) {
    std::cout << "Vehicle not initialized, exiting.\n";
    return -1;
  }

  // Obtain Control Authority
  vehicle->obtainCtrlAuthority(functionTimeout);

  // Display interactive prompt
  std::cout
      << "| Available commands:                                            |"
      << std::endl;
  std::cout
      << "| [a] Monitored Takeoff + Landing                                |"
      << std::endl;
  std::cout
      << "| [b] Monitored Takeoff + Position Control + Landing             |"
      << std::endl;
  std::cout << "| [c] Monitored Takeoff + Position Control + Force Landing "
               "Avoid Ground  |"
            << std::endl;

  char inputChar;
  std::cin >> inputChar;

  switch (inputChar) {
    case 'a':
      monitoredTakeoff(vehicle);
      monitoredLanding(vehicle);
      break;
    case 'b':
      monitoredTakeoff(vehicle);
      moveByPositionOffset(vehicle, 0, 6, 6, 30);
      moveByPositionOffset(vehicle, 6, 0, -3, -30);
      moveByPositionOffset(vehicle, -6, -6, 0, 0);
      monitoredLanding(vehicle);
      break;

    /*! @NOTE: case 'c' only support for m210 V2*/
    case 'c':
      /*! Turn off rtk switch */
      ErrorCode::ErrorCodeType ret;
      ret = vehicle->flightController->setRtkEnableSync(
          FlightController::RtkEnabled::RTK_DISABLE, 1);
      if (ret != ErrorCode::SysCommonErr::Success) {
        DSTATUS("Turn off rtk switch failed, ErrorCode is:%8x", ret);
      } else {
        DSTATUS("Turn off rtk switch successfully");
      }

      /*!  Take off */
      monitoredTakeoff(vehicle);

      /*! Move to higher altitude */
      moveByPositionOffset(vehicle, 0, 0, 30, 0);

      /*! Move a short distance*/
      moveByPositionOffset(vehicle, 10, 0, 0, -30);

      /*! Set aircraft current position as new home location */
      setNewHomeLocation(vehicle);

      /*! Set new go home altitude */
      setGoHomeAltitude(vehicle, 50);

      /*! Move to another position */
      moveByPositionOffset(vehicle, 40, 0, 0, 0);

      /*! go home and  confirm landing */
      goHomeAndConfirmLanding(vehicle, 1);
      break;

    default:
      break;
  }

  return 0;
}