/*! @file main.cpp
 *  @version 1.0.0
 *  @date 17.04.2021
 *
 *  @brief
 *  Our brief
*/

#include "flight_control_sample.hpp"                                  
#include "flight_sample.hpp"
#include "MyLibrary.hpp"                                                                      // Library for test purpose
#include <Camera.hpp>                                                                         // ZED library inclusion (INSTALLED LIBRARY)
#include "FlightLibrary.hpp"                                                                  // Library for Flight control and Reading Data

#define MAX_BUF 1024

using namespace sl;                                                                           // Namespace definition for ZED library 
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;
using namespace MyNameSpaceFirst;                                                             // Namespace definition (OPTIONAL)
using namespace MyNameSpaceFirst::MyNameSpaceSecond;                                          // Namespace definition (OPTIONAL)
using std::chrono::high_resolution_clock;                                                     // Namespaces for measuring execution speed
using std::chrono::duration_cast;                                                             // Namespaces for measuring execution speed
using std::chrono::duration;                                                                  // Namespaces for measuring execution speed
using std::chrono::milliseconds;                                                              // Namespaces for measuring execution speed
using namespace FlightLibrary;                                        

MyClassOne::MyStructOne InstanceStructOne;                                                    // Create instance of struct (utilizing namespace)
MyClassOne::MyEnumOne myenumone;                                                              // Create instance of enum (utilizing namespace)
MyControlClass::ControlStruct controlstructmain;                                              // Create instance of struct (utilizing namespace)
MyControlClass::ControlEnum controlenummain;                                                  // Create instance of enum (utilizing namespace)

//VehicleStatus::DisplayMode displaystatus;

int main(int argc, char** argv) 
{
  //*********************************************************************************************************************************************
  // Measuring execution time
  auto t1 = high_resolution_clock::now();                                                     // Run function to measure execution time

    //*********************************************************************************************************************************************
  // MyLibrary & FlightLibrary integration
  /*

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
  classutilizingpointers->FunctionThatAllocatesMemoryInTwoDifferentWays();

  ClassUtilizingPointers::ClassInClassUtilizingPointers* classinclassutilizingpointers;       // Create instance of class pointer and point to class
  classinclassutilizingpointers->FunctionInClassInClassUtilizingPointers();                   // Call method(function) of class using pointers

  classutilizingpointers->myclasstwo->PrintVectorCalculations(VectorInstanceToBePassed);      // Call method(function) of class using pointer from one class to other class
  classutilizingpointers->myclassone->PrintFromFirstClass();                                  // Call method(function) of class using pointer from one class to other class

  TelemetryClass::Quaternion quaternion;                                                                // Create instance of struct that is going to hold returned struct data
  quaternion = classutilizingpointers->telemetryclass->GetQuaternion();                                 // Run method(function) to get data and retunr it to struct        
  std::cout << "Quaternion data " <<quaternion.x<<" "<<quaternion.y<<" "<<quaternion.z<< std::endl;     // Print returned struct data

  classutilizingpointers->FunctionThatCreatesFourThreads();                                   // Call function that is accessed using class pointers
  classutilizingpointers->StringHandlingAndConvertingFunction();                              // Call function that is accessed using class pointers

  auto t2 = high_resolution_clock::now();                                                     // Call function to measure exectuion time
  duration<double, std::milli> ms_double = t2 - t1;                                           // Getting number of milliseconds as a double
  std::cout << "Execution time in " << ms_double.count() << " ms" << std::endl;               // Print

  std::cout << "Working" << std::endl;
  usingConstAndExplict usingconstandexplicit;
  usingConstAndExplict::A a1 = 1;      // OK: copy-initialization selects A::A(int)
  usingConstAndExplict::A a2(2);       // OK: direct-initialization selects A::A(int)
  usingConstAndExplict::A a3 {4, 5};   // OK: direct-list-initialization selects A::A(int, int)
  usingConstAndExplict::A a4 = {4, 5}; // OK: copy-list-initialization selects A::A(int, int)
    
  usingConstAndExplict::A a5 = (usingConstAndExplict::A)1;   // OK: explicit cast performs static_cast
  if (a1) cout << "true" << endl; // OK: A::operator bool()
  bool na1 = a1; // OK: copy-initialization selects A::operator bool()
  bool na2 = static_cast<bool>(a1); // OK: static_cast performs direct-initialization
    
//  usingConstAndExplict::B b1 = 1;      // error: copy-initialization does not consider B::B(int)
  usingConstAndExplict::B b2(2);       // OK: direct-initialization selects B::B(int)
  usingConstAndExplict::B b3 {4, 5};   // OK: direct-list-initialization selects B::B(int, int)
//  usingConstAndExplict::B b4 = {6, 7}; // error: copy-list-initialization does not consider B::B(int,int)
    
  usingConstAndExplict::B b5 = (usingConstAndExplict::B)1;   // OK: explicit cast performs static_cast
  if (b5) cout << "true " << endl; // OK: B::operator bool()
//  bool nb1 = b2; // error: copy-initialization does not consider B::operator bool()
  bool nb2 = static_cast<bool>(b2); // OK: static_cast performs direct-initialization

  std::string str = "Hello, ";
  str.operator+=("world");                        // same as str += "world";
  operator<<(operator<<(std::cout, str) , '\n');  // same as std::cout << str << '\n';
                                                  // (since C++17) except for sequencin
  std::cout << usingconstandexplicit.publicFloat << std::endl;

  usingConstAndExplict x(3,3);
  usingConstAndExplict y(2,2);
  usingConstAndExplict z = x + y; // calls complx::operator+()  

  */

  FlightTelemetry* flighttelemetry;                                             // Instantiate FlightTelemetry class object pointer
  FlightCommander* flightcommander;                                             // Instantiate FlightCommander class object pointer
  
  //signal(SIGINT, SafetyFunction);
  //*********************************************************************************************************************************************
  // ZED integration
  
  //sl::Camera zed;                                                               // Create instance(object) of class from the ZED library
  //flighttelemetry->openCameraZed(zed);                                          // Open Zed camera
  
  //*********************************************************************************************************************************************
  // POZYX integration
  /*
  Settings:
  Data BitRate from 110 kbps to 6.81 Mbps
  Preamble from 1024 kbps to 128kbps
  */
    FlightTelemetry::UwbStruct uwbstruct;
    int fd;
    char *FifoPipe = "Pipe.fifo";
    char buf[MAX_BUF];
    fd = open(FifoPipe, O_RDONLY);                                                            // Open FIFO pipe for reading incoming
    std::cout << "Testing Pozyx" << std::endl;
    for (int i = 0; i < 1; i++)
    {
      uwbstruct = flighttelemetry->GetUwbPositionData(fd, buf);
      std::cout << "X" <<uwbstruct.x<< " Y" <<uwbstruct.y<< " Z" <<uwbstruct.z<< std::endl;
      sleep(1);
    }
    std::cout << "Pozyx works - Ready to Fly" << std::endl;
    
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

  //*********************************************************************************************************************************************
  // Loop

  while(true)
  {
    // Display interactive prompt
    std::cout << std::endl
        << "| Available   commands:                                           |"
        << std::endl;
    std::cout
        << "| [a] Arming  Command                                             |"
        << std::endl;
    std::cout
        << "| [l] Land    Command                                             |"
        << std::endl;
    std::cout
        << "| [t] Takeoff Command                                             |"
        << std::endl;
    std::cout
        << "| [m] Move    Command                                             |"
        << std::endl;
    std::cout
        << "| [n] Move    Command 2                                           |"
        << std::endl;
    std::cout
        << "| [d] Data    Read                                                |"
        << std::endl;
    std::cout
        << "| [e] End     Session                                             |"
        << std::endl;

    char inputChar;
    int defineParameters  = 0;      // This used to define paramters    
    float Xtarget         = 3.5;    // Initial value if not defined manually (X target offset from localization system origin)
    float Ytarget         = 2.6;    // Initial value if not defined manually (Y target offset from localization system origin)
    float Ztarget         = 0.5;    // Initial value if not defined manually (Z target offset from localization system origin)
    float YawTarget       = 102.6;  // Initial value if not defined manually (Yaw target offset from localization system origin)
    float pgain           = 0.05;    // Initial value if not defined manually
    float igain           = 0.7;    // Initial value if not defined manually
    float dgain           = 0.7;    // Initial value if not defined manually
    int timeoutInMilSec   = 10000;  // Initial value if not defined manually
    float maxPitchDeg     = 5;      // Initial value if not defined manually (Max pitch in degrees)
    float maxRollDeg      = 5;      // Initial value if not defined manually (Max roll in degrees)

    std::cin >> inputChar;

    switch (inputChar) {

      case 'a':
        vehicle->control->armMotors(1);                                         // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'l':
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 't' :
        vehicle->control->takeoff(1);
        // Takeoff code here
        /*
        Yaw is fixed to defined degree (12.5 degrees is value to rotate to UWB y-axis direction) (TEST THIS 26.5 when using inside source file movebyposition)
        Y is inversed so that -y = pitch forward
        X is not inversed so that -x = roll left
        Z is locked to defined altidude from the takeoff point -> if z now is 2 and z next is 0.5 then vehicle will come down to 0.5m from the takeoff point
        */
        //vehicle->control->takeoff(1);
        /*
        for(int i = 0; i < 4000; i++)
        {
          vehicle->control->attitudeAndVertPosCtrl(0, 0, 12.5, 1.5);                    // -63 is facing away from window
          usleep(1000);
        }
        std::cout << "Takeoff finished" << std::endl;
        */
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'm' :
        // Move code #1 here
        std::cout << " " << std::endl;
        std::cout << "Input YawTarget: ";
        std::cin >> YawTarget;
        for(int i = 0; i < 3000; i++)
        {
          //vehicle->control->attitudeAndVertPosCtrl(0, 0, YawTarget, 1.5);                    // -63 is facing away from window
          //vehicle->control->velocityAndYawRateCtrl(0, 0, 0, 0);
          vehicle->control->positionAndYawCtrl(0, 3, 1.5, 0);
          usleep(1000);
        }        
        std::cout << "Finished" << std::endl;
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'n' :
        // Move code #2 here
        /*
        std::cout << " " << std::endl;
        std::cout << "Input Xtarget: ";
        std::cin >> Xtarget;
        std::cout << " " << std::endl;
        std::cout << "Input Ytarget: ";
        std::cin >> Ytarget;
        std::cout << " " << std::endl;
        std::cout << "Input Ztarget: ";
        std::cin >> Ztarget;    
        std::cout << " " << std::endl;
        std::cout << "Input YawTarget: ";
        std::cin >> YawTarget;            
        */
        std::cout << " " << std::endl;
        std::cout << "Define paramters? (1 = YES, else NO) ";
        std::cin >> defineParameters;
        
        if(defineParameters == 1)
        {
        std::cout << " " << std::endl;
        std::cout << "Input pgain: ";
        std::cin >> pgain;
        std::cout << " " << std::endl;
        std::cout << "Input igain: ";
        std::cin >> igain;
        std::cout << " " << std::endl;
        std::cout << "Input dgain: ";
        std::cin >> dgain;
        std::cout << " " << std::endl;
        std::cout << "Input timeoutInMilSec: ";
        std::cin >> timeoutInMilSec;
        std::cout << " " << std::endl;
        std::cout << "Input maxRollDeg: ";
        std::cin >> maxRollDeg;
        std::cout << " " << std::endl;
        std::cout << "Input maxPitchDeg: ";
        std::cin >> maxPitchDeg;
        std::cout << " " << std::endl;                                    
        }           
                    
        moveByPositionOffset(vehicle, Xtarget, Ytarget, Ztarget, YawTarget, pgain, igain, dgain, timeoutInMilSec, maxRollDeg, maxPitchDeg, 0.5, 1.0);    // This position (vehicle, 3.5, 2.6, 0, 12.5) is taped to the floor
        std::cout << "Finished" << std::endl;
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
      break;
      case 'b' :
        // Move code #3 here 
        std::cout << " " << std::endl;
        std::cout << "Input Xtarget: ";
        std::cin >> Xtarget;
        std::cout << " " << std::endl;
        std::cout << "Input Ytarget: ";
        std::cin >> Ytarget;
        std::cout << " " << std::endl;
        std::cout << "Input Ztarget: ";
        std::cin >> Ztarget;    
        std::cout << " " << std::endl;
        std::cout << "Input YawTarget: ";
        std::cin >> YawTarget;           
        std::cout << " " << std::endl;
        std::cout << "Input pgain: ";
        std::cin >> pgain;
        std::cout << " " << std::endl;
        std::cout << "Input igain: ";
        std::cin >> igain;
        std::cout << " " << std::endl;
        std::cout << "Input dgain: ";
        std::cin >> dgain;
        std::cout << " " << std::endl;                                
        moveByPositionOffset(vehicle, Xtarget, Ytarget, Ztarget, YawTarget, pgain, igain, dgain, timeoutInMilSec, maxRollDeg, maxPitchDeg, 0.5, 1.0);    // This position (vehicle, 3.5, 2.6, 0, 12.5) is taped to the floor
        std::cout << "Finished" << std::endl;
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'd' :
        // Data code here
        //flighttelemetry->GetQuaternionData(vehicle);
        flighttelemetry->GetGlobalPositionData(vehicle, 1);
        break;

    }
    if(inputChar == 'e')                                                        // End session here
    {
      break;
    }
  }
  

  auto t2 = high_resolution_clock::now();                                                     // Call function to measure exectuion time
  duration<double, std::milli> ms_double = t2 - t1;                                           // Getting number of milliseconds as a double
  std::cout << "Execution time in " << ms_double.count() << " ms" << std::endl;               // Print

  close(fd);                                                                                  // Pozyx Integration (FIFO pipe closed)
  //zed.close();                                                                                // Close the camera

  return 0;
}



