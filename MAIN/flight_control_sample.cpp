/*! @file flight_control_sample.cpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  Flight Control API usage in a Linux environment.
 *  Provides a number of helpful additions to core API calls,
 *  especially for position control, attitude control, takeoff,
 *  landing.
 *
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * Edited With VIM
 */

#include "flight_control_sample.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;
using namespace FlightLibrary;

using std::chrono::high_resolution_clock;                                                     // Namespaces for measuring execution speed
using std::chrono::duration_cast;                                                             // Namespaces for measuring execution speed
using std::chrono::duration;                                                                  // Namespaces for measuring execution speed
using std::chrono::milliseconds;                                                              // Namespaces for measuring execution speed


/*! Position Control. Allows you to set an offset from your current
    location. The aircraft will move to that position and stay there.
    Typical use would be as a building block in an outer loop that does not
    require many fast changes, perhaps a few-waypoint trajectory. For smoother
    transition and response you should convert your trajectory to attitude
    setpoints and use attitude control or convert to velocity setpoints
    and use velocity control.
!*/

bool
moveByPositionOffset(Vehicle *vehicle, float xOffsetDesired,
                     float yOffsetDesired, float zOffsetDesired,
                     float yawDesired, float pgain, float igain, float dgain, int timeoutInMilSec, float maxRoll, float maxPitch, float posThresholdInM,
                     float yawThresholdInDeg)  // Gain variables added
{
  // Set timeout: this timeout is the time you allow the drone to take to finish
  // the
  // mission
  int responseTimeout              = 1;
  int controlFreqInHz              = 50; // Hz                                      // Originally 50Hz, changed to other frequency
  int cycleTimeInMs                = 1000 / controlFreqInHz;
  int outOfControlBoundsTimeLimit  = 10 * cycleTimeInMs; // 10 cycles
  int withinControlBoundsTimeReqmt = 50 * cycleTimeInMs; // 50 cycles
  int pkgIndex;
  std::cout << std::setprecision(2) << std::fixed;                                  // Set the printed float decimals
  

  
  double pTermX                   = 0;  // PID start
  double iTermX                   = 0;
  double dTermX                   = 0;
  double iStateX                  = 0;
  
  double pTermY                   = 0;  
  double iTermY                   = 0;
  double dTermY                   = 0;
  double iStateY                  = 0;

  double windupLimit              = 10;
  double veloFctr                 = 1;    // variable to adjust target speed
  double speedLimitFctr           = 0.6;  // another variable to adjust target speed

  /*
  int maxPitch                    = 2;
  int maxRoll                     = 2;  
  */
  double curPosX = 0;
  double curPosY = 0;
  double lastPosX = 0;
  double lastPosY = 0;
  double curVeloX  = 0;
  double curVeloY  = 0;
  double lastVeloX  = 0;
  double lastVeloY  = 0;

  //double positionCmd = 0;
  double posCmdX = 0;
  double posCmdY = 0;
  double pitchCmd = 0;
  double rollCmd = 0;               // PID end
  

  //@todo: remove this once the getErrorCode function signature changes
  char func[50];
  
  // Global position retrieved via broadcast
  Telemetry::GlobalPosition currentBroadcastGP;
  Telemetry::GlobalPosition originBroadcastGP;

  FlightTelemetry* flighttelemetry;                                                         // Create an object pointer of FlightTelemetry
  FlightTelemetry::UwbStruct uwbstruct;                                                     // Instiate struct
  /*
  sl::Camera zed;                                                                           // ++++ Zed class object instance
  sl::Pose camera_path;                                                                     // ++++ Zed instance
  sl::float3 translation;                                                                   // ++++ Zed instance
  sl::float3 rotation;                                                                      // ++++ Zed instance
  std::vector<float> returnedFloat;
  std::vector<float> storedFloat;
  std::pair<sl::float3 , sl::float3> ReturnPairPosRot;
  flighttelemetry->openCameraZed(zed);                                                      // ++++ Open Zed camera
  */
  int fd;                                                                                   // Create integer for piping
  char *FifoPipe = "Pipe.fifo";                                                             // Create a pipe
  char buf[MAX_BUF];                                                                        // Define maximum buffer size
  fd = open(FifoPipe, O_RDONLY);                                                            // Open FIFO pipe for reading incoming

  // Convert position offset from first position to local coordinates
  Telemetry::Vector3f localOffset;

  if (!vehicle->isM100() && !vehicle->isLegacyM600())
  {
    // Get the broadcast GP since we need the height for zCmd
    currentBroadcastGP = vehicle->broadcast->getGlobalPosition();
  }
  else
  {
    currentBroadcastGP = vehicle->broadcast->getGlobalPosition();
    originBroadcastGP  = currentBroadcastGP;
    localOffsetFromGpsOffset(vehicle, localOffset,
                             static_cast<void*>(&currentBroadcastGP),
                             static_cast<void*>(&originBroadcastGP));
  }
  /*
  for(int i = 0; i < 10000; i++)
  {
  ReturnPairPosRot = flighttelemetry->getPositionZed(zed, camera_path, translation, rotation, ReturnPairPosRot);     // ++++ Get Zed position and rotation data
  std::cout << "Zed x translation " << ReturnPairPosRot.first[0] << " Zed x rotation " << ReturnPairPosRot.second[0] << std::endl;  // ++++ Print Zed 
  uwbstruct = flighttelemetry->GetUwbPositionData(fd, buf);                     // Get Uwb position data and store to uwbstruct
  }
  */                                  
  for (int i = 0; i < 2; i++)                                                   // Testing purpose (Optional)
  {
    uwbstruct = flighttelemetry->GetUwbPositionData(fd, buf);
    std::cout << "X" <<uwbstruct.pX<< " Y" <<uwbstruct.pY<< " Z" <<uwbstruct.pZ<< std::endl;
    //sleep(1);
  }

  double lastPosX = uwbstruct.pX;  // PID start
  double lastPosY = uwbstruct.pY;  // PID end
  
  /* 
  // Original implementation
  // Get initial offset. We will update this in a loop later.
  double xOffsetRemaining = xOffsetDesired - localOffset.x;
  double yOffsetRemaining = yOffsetDesired - localOffset.y;
  double zOffsetRemaining = zOffsetDesired - localOffset.z;
  */

  // Get initial offset. We will update this in a loop later.
  double xOffsetRemaining = (xOffsetDesired - uwbstruct.pX);                     // Set offset remaining using localoffset = uwbstruct
  double yOffsetRemaining = (yOffsetDesired - uwbstruct.pY);                     // Set offset remaining using localoffset = uwbstruct (y control function is inversed)
  double zOffsetRemaining = zOffsetDesired - uwbstruct.pZ;                       // Set offset remaining using localoffset = uwbstruct


  // To check if error is positive or negative, needed later on when taking square root
  if (xOffsetRemaining < 0){
      int posNegX = -1;
  } else {
      int posNegX = 1;
  }

  curVeloX += uwbstruct.aX * cycleTimeInMs * 1000;  // @TODO: Check acceleration directions
  curVeloY += uwbstruct.aY * cycleTimeInMs * 1000;

  double targetVeloX = xOffsetRemaining * veloFctr;
  double targetVeloY = yOffsetRemaining * veloFctr;

  double xyRelationship = abs(targetVeloY/targetVeloX); // relationship between X and Y target velocities, needed later on

  double totalVelo = sqrt(pow(targetVeloX,2) + pow(targetVeloY,2));
  double speedLimit = pow(sqrt(pow(xOffsetRemaining,2) + pow(yOffsetRemaining,2)),0.6) * speedLimitFctr;

  // Limiting speed when close to target = brake
  if (totalVelo > speedLimit) {
    targetVeloX = sqrt(pow(speedLimit,2) / (1 + xyRelationship)) * posNegX;
    targetVeloY = targetVeloX * xyRelationship;
  }
  
  double veloErrorX = targetVeloX - curVeloX;
  double veloErrorY = targetVeloY - curVeloY;


  // Conversions
  double yawDesiredRad     = DEG2RAD * yawDesired;
  double yawThresholdInRad = DEG2RAD * yawThresholdInDeg;
  
  // Quaternion retrieved via broadcast
  Telemetry::Quaternion broadcastQ;

  double yawInRad;
  double yawInDeg;

  broadcastQ = vehicle->broadcast->getQuaternion();
  yawInRad   = toEulerAngle((static_cast<void*>(&broadcastQ))).z / DEG2RAD;

  yawInRad   = PI/2 - yawInRad + ((12.6/180)*PI); // PID
  yawInDeg   = (yawInRad / (2*PI)) * 360;
  
  int   elapsedTimeInMs     = 0;
  int   withinBoundsCounter = 0;
  int   outOfBounds         = 0;
  int   brakeCounter        = 0;
  int   speedFactor         = 2;
  float xCmd, yCmd, zCmd;

  /*! Calculate the inputs to send the position controller. We implement basic
   *  receding setpoint position control and the setpoint is always 1 m away
   *  from the current position - until we get within a threshold of the goal.
   *  From that point on, we send the remaining distance as the setpoint.
   */

  if (xOffsetDesired > 0)
    xCmd = (xOffsetDesired < speedFactor) ? xOffsetDesired : speedFactor;
  else if (xOffsetDesired < 0)
    xCmd =
      (xOffsetDesired > -1 * speedFactor) ? xOffsetDesired : -1 * speedFactor;
  else
    xCmd = 0;

  if (yOffsetDesired > 0)
    yCmd = (yOffsetDesired < speedFactor) ? yOffsetDesired : speedFactor;
  else if (yOffsetDesired < 0)
    yCmd =
      (yOffsetDesired > -1 * speedFactor) ? yOffsetDesired : -1 * speedFactor;
  else
    yCmd = 0;

  if (!vehicle->isM100() && !vehicle->isLegacyM600())
  {
    zCmd = currentBroadcastGP.height + zOffsetDesired; //Since subscription cannot give us a relative height, use broadcast.
  }
  else
  {
    zCmd = currentBroadcastGP.height + zOffsetDesired;
  }

  //! Main closed-loop receding setpoint position control
  while (elapsedTimeInMs < timeoutInMilSec)
  {
    auto t1 = high_resolution_clock::now();                                                     // Run function to measure execution time

    /*
    vehicle->control->positionAndYawCtrl(xCmd, yCmd, zCmd,
                                         yawDesiredRad / DEG2RAD);
    */
    vehicle->control->attitudeAndVertPosCtrl(rollCmd, pitchCmd * (-1.0), yawDesired, zCmd);
                                
            

    usleep(cycleTimeInMs * 1000);
    elapsedTimeInMs += cycleTimeInMs;

    //! Get current position in required coordinates and units
    if (!vehicle->isM100() && !vehicle->isLegacyM600())
    {
      // Get the broadcast GP since we need the height for zCmd
      
      currentBroadcastGP = vehicle->broadcast->getGlobalPosition();
    }
    else
    {
      broadcastQ         = vehicle->broadcast->getQuaternion();
      yawInRad           = toEulerAngle((static_cast<void*>(&broadcastQ))).z;
      currentBroadcastGP = vehicle->broadcast->getGlobalPosition();
      localOffsetFromGpsOffset(vehicle, localOffset,
                               static_cast<void*>(&currentBroadcastGP),
                               static_cast<void*>(&originBroadcastGP));
    }
    /*
    // This is original offset declaration (Remember to change initial offset also)
    //! See how much farther we have to go
    xOffsetRemaining = xOffsetDesired - localOffset.x;
    yOffsetRemaining = yOffsetDesired - localOffset.y;
    zOffsetRemaining = zOffsetDesired - localOffset.z;
    */

    uwbstruct = flighttelemetry->GetUwbPositionData(fd, buf);                       // Get Uwb postion data and store to uwbstruct
    xOffsetRemaining = (xOffsetDesired - uwbstruct.pX);                            // Set offset remaining using localoffset = uwbstruct
    yOffsetRemaining = ((yOffsetDesired - uwbstruct.pY));                          // Set offset remaining using localoffset = uwbstruct
    zOffsetRemaining = zOffsetDesired - uwbstruct.pZ;                              // Set offset remaining using localoffset = uwbstruct

    if (xOffsetRemaining < 0){
        posNegX = -1;
    } else {
        posNegX = 1;
    }

    curVeloX += uwbstruct.aX * cycleTimeInMs * 1000;  // @TODO: Check acceleration directions
    curVeloY += uwbstruct.aY * cycleTimeInMs * 1000;

    targetVeloX = xOffsetRemaining * veloFctr;
    targetVeloY = yOffsetRemaining * veloFctr;

    xyRelationship = abs(targetVeloY/targetVeloX);

    totalVelo = sqrt(pow(targetVeloX,2) + pow(targetVeloY,2));
    speedLimit = pow(sqrt(pow(xOffsetRemaining,2) + pow(yOffsetRemaining,2)),0.6) * speedLimitFctr;

    if (totalVelo > speedLimit) {
      targetVeloX = sqrt(pow(speedLimit,2) / (1 + xyRelationship)) * posNegX;
      targetVeloY = targetVeloX * xyRelationship;
    }
    
    veloErrorX = targetVeloX - curVeloX;
    veloErrorY = targetVeloY - curVeloY;


    yawInRad   = PI/2 - yawInRad + ((12.6/180)*PI); // PID
    yawInDeg   = (yawInRad / (2*PI)) * 360;

    // ============= PID-controller =============

    pTermX = pgain * veloErrorX;
    pTermY = pgain * veloErrorY;

    iStateX += veloErrorX * cycleTimeInMs/1000;
    iStateY += veloErrorY * cycleTimeInMs/1000;

    // Windup guard
    if (iStateX > windupLimit) {
        iStateX = windupLimit;
    } else if (iStateX < -windupLimit) {
        iStateX = -windupLimit;
    } 
    if (iStateY > windupLimit) {
        iStateY = windupLimit;
    } else if (iStateY < -windupLimit) {
        iStateY = -windupLimit;
    } 

    iTermX = igain * iStateX;
    iTermY = igain * iStateY;


    dTermX = (dgain * (curVeloX - lastVeloX)) / cycleTimeInMs; 
    dTermY = (dgain * (curVeloY - lastVeloY)) / cycleTimeInMs;

    positionCmdX = pTermX + iTermX + dTermX;
    positionCmdY = pTermY + iTermY + dTermY;
    
    pitchCmd = cos(yawInRad)*positionCmdX + sin(yawInRad)*positionCmdY;      // needed pitch of the drone
    rollCmd = sin(yawInRad)*positionCmdX - cos(yawInRad)*positionCmdY;       // needed roll of the drone


    // limiting Pitch
    if (pitchCmd > maxPitch){
        pitchCmd = maxPitch;
    } else if (pitchCmd < -maxPitch){
        pitchCmd = -maxPitch;
    }

    // limiting Roll    
    if (rollCmd > maxRoll){
        rollCmd = maxRoll;
    } else if (rollCmd < -maxRoll){
        rollCmd = -maxRoll;
    }    

    /*
    //! See if we need to modify the setpoint
    if (std::abs(xOffsetRemaining) < speedFactor)
    {
      xCmd = xOffsetRemaining;
    }
    if (std::abs(yOffsetRemaining) < speedFactor)
    {
      yCmd = yOffsetRemaining;
    }

    if (vehicle->isM100() && std::abs(xOffsetRemaining) < posThresholdInM &&
        std::abs(yOffsetRemaining) < posThresholdInM &&
        std::abs(yawInRad - yawDesiredRad) < yawThresholdInRad)
    {
      //! 1. We are within bounds; start incrementing our in-bound counter
      withinBoundsCounter += cycleTimeInMs;
    }
    else if (std::abs(xOffsetRemaining) < posThresholdInM &&
             std::abs(yOffsetRemaining) < posThresholdInM &&
             std::abs(zOffsetRemaining) < posThresholdInM &&
             std::abs(yawInRad - yawDesiredRad) < yawThresholdInRad)
    {
      //! 1. We are within bounds; start incrementing our in-bound counter
      withinBoundsCounter += cycleTimeInMs;
    }
    else
    {
      if (withinBoundsCounter != 0)
      {
        //! 2. Start incrementing an out-of-bounds counter
        outOfBounds += cycleTimeInMs;
      }
    }
    //! 3. Reset withinBoundsCounter if necessary
    if (outOfBounds > outOfControlBoundsTimeLimit)
    {
      withinBoundsCounter = 0;
      outOfBounds         = 0;
    }
    //! 4. If within bounds, set flag and break
    if (withinBoundsCounter >= withinControlBoundsTimeReqmt)
    {
      break;
    }
    */


    lastVeloX = curVeloX; // PID start
    lastVeloY = curVeloY; // PID end
    auto t2 = high_resolution_clock::now();                                                     // Call function to measure exectuion time
    duration<double, std::milli> ms_double = t2 - t1;                                           // Getting number of milliseconds as a double
    std::cout 
    // << "lO.x " << localOffset.x << "    lO.y " << localOffset.y << "    lO.z " << localOffset.z                        // localOffset is used only with GPS data
    << "lO.x " << uwbstruct.pX << "    lO.y " << uwbstruct.pY << "    lO.z " << uwbstruct.pZ << "    lO.YawDeg " << yawInDeg   // uwbstruct is used only with UWB data
    << "         xOR " << xOffsetRemaining << "    yOR " << yOffsetRemaining << "    zOR " << zOffsetRemaining
    << "         rollCmd " << rollCmd << "    pitchCmd " << pitchCmd << "    zCmd " << zCmd
    << "         YawD " << yawDesired << " ExeT " << 1/(ms_double.count()/1000)
    // << "         Alt " << currentBroadcastGP.altitude << "    Lat " << currentBroadcastGP.latitude << "    Lon " << currentBroadcastGP.longitude
    // << "         GPS Health " << currentBroadcastGP.health
    << std::endl;

  }

  //! Set velocity to zero, to prevent any residual velocity from position
  //! command
  /*
  if (!vehicle->isM100() && !vehicle->isLegacyM600())
  {
    while (brakeCounter < withinControlBoundsTimeReqmt)
    {
      vehicle->control->emergencyBrake();
      usleep(cycleTimeInMs * 10);
      brakeCounter += cycleTimeInMs;
    }
  }
  */
  if (elapsedTimeInMs >= timeoutInMilSec)
  {
    std::cout << "Task timeout!\n";
    /*
    if (!vehicle->isM100() && !vehicle->isLegacyM600())
    {
      ACK::ErrorCode ack =
        vehicle->subscribe->removePackage(pkgIndex, responseTimeout);
      if (ACK::getError(ack))
      {
        std::cout << "Error unsubscribing; please restart the drone/FC to get "
                     "back to a clean state.\n";
      }
    }
    */
    // return ACK::FAIL;
  }
  /*
  if (!vehicle->isM100() && !vehicle->isLegacyM600())
  {
    ACK::ErrorCode ack =
      vehicle->subscribe->removePackage(pkgIndex, responseTimeout);
    if (ACK::getError(ack))
    {
      std::cout
        << "Error unsubscribing; please restart the drone/FC to get back "
           "to a clean state.\n";
    }
  }
  */

  //return ACK::SUCCESS;
}

// Helper Functions

/*! Very simple calculation of local NED offset between two pairs of GPS
/coordinates.
    Accurate when distances are small.
!*/
void
localOffsetFromGpsOffset(Vehicle* vehicle, Telemetry::Vector3f& deltaNed,
                         void* target, void* origin)
{
  Telemetry::GPSFused*       subscriptionTarget;
  Telemetry::GPSFused*       subscriptionOrigin;
  Telemetry::GlobalPosition* broadcastTarget;
  Telemetry::GlobalPosition* broadcastOrigin;
  double                     deltaLon;
  double                     deltaLat;

  if (!vehicle->isM100() && !vehicle->isLegacyM600())
  {
    subscriptionTarget = (Telemetry::GPSFused*)target;
    subscriptionOrigin = (Telemetry::GPSFused*)origin;
    deltaLon   = subscriptionTarget->longitude - subscriptionOrigin->longitude;
    deltaLat   = subscriptionTarget->latitude - subscriptionOrigin->latitude;
    deltaNed.x = deltaLat * C_EARTH;
    deltaNed.y = deltaLon * C_EARTH * cos(subscriptionTarget->latitude);
    deltaNed.z = subscriptionTarget->altitude - subscriptionOrigin->altitude;
  }
  else
  {
    broadcastTarget = (Telemetry::GlobalPosition*)target;
    broadcastOrigin = (Telemetry::GlobalPosition*)origin;
    deltaLon        = broadcastTarget->longitude - broadcastOrigin->longitude;
    deltaLat        = broadcastTarget->latitude - broadcastOrigin->latitude;
    deltaNed.x      = deltaLat * C_EARTH;
    deltaNed.y      = deltaLon * C_EARTH * cos(broadcastTarget->latitude);
    deltaNed.z      = broadcastTarget->altitude - broadcastOrigin->altitude;
  }
}

Telemetry::Vector3f
toEulerAngle(void* quaternionData)
{
  Telemetry::Vector3f    ans;
  Telemetry::Quaternion* quaternion = (Telemetry::Quaternion*)quaternionData;

  double q2sqr = quaternion->q2 * quaternion->q2;
  double t0    = -2.0 * (q2sqr + quaternion->q3 * quaternion->q3) + 1.0;
  double t1 =
    +2.0 * (quaternion->q1 * quaternion->q2 + quaternion->q0 * quaternion->q3);
  double t2 =
    -2.0 * (quaternion->q1 * quaternion->q3 - quaternion->q0 * quaternion->q2);
  double t3 =
    +2.0 * (quaternion->q2 * quaternion->q3 + quaternion->q0 * quaternion->q1);
  double t4 = -2.0 * (quaternion->q1 * quaternion->q1 + q2sqr) + 1.0;

  t2 = (t2 > 1.0) ? 1.0 : t2;
  t2 = (t2 < -1.0) ? -1.0 : t2;

  ans.x = asin(t2);
  ans.y = atan2(t3, t4);
  ans.z = atan2(t1, t0);

  return ans;
}

bool startGlobalPositionBroadcast(Vehicle* vehicle)
{
  uint8_t freq[16];

  /* Channels definition for A3/N3/M600
   * 0 - Timestamp
   * 1 - Attitude Quaternions
   * 2 - Acceleration
   * 3 - Velocity (Ground Frame)
   * 4 - Angular Velocity (Body Frame)
   * 5 - Position
   * 6 - GPS Detailed Information
   * 7 - RTK Detailed Information
   * 8 - Magnetometer
   * 9 - RC Channels Data
   * 10 - Gimbal Data
   * 11 - Flight Status
   * 12 - Battery Level
   * 13 - Control Information
   */
  freq[0]  = DataBroadcast::FREQ_HOLD;
  freq[1]  = DataBroadcast::FREQ_HOLD;
  freq[2]  = DataBroadcast::FREQ_HOLD;
  freq[3]  = DataBroadcast::FREQ_HOLD;
  freq[4]  = DataBroadcast::FREQ_HOLD;
  freq[5]  = DataBroadcast::FREQ_50HZ; // This is the only one we want to change
  freq[6]  = DataBroadcast::FREQ_HOLD;
  freq[7]  = DataBroadcast::FREQ_HOLD;
  freq[8]  = DataBroadcast::FREQ_HOLD;
  freq[9]  = DataBroadcast::FREQ_HOLD;
  freq[10] = DataBroadcast::FREQ_HOLD;
  freq[11] = DataBroadcast::FREQ_HOLD;
  freq[12] = DataBroadcast::FREQ_HOLD;
  freq[13] = DataBroadcast::FREQ_HOLD;

  ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreq(freq, 1);
  if (ACK::getError(ack))
  {
    ACK::getErrorCodeMessage(ack, __func__);
    return false;
  }
  else
  {
    return true;
  }
}
