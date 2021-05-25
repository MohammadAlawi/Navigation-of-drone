#include <stdio>
#include <cmath>

const double PI = 3.14159265358979323846;

// ------ Variables ------
// @TODO: change the values of some variables

double curPosX  = 80;               // current position along x-axis of room
double curPosY  = 80;               // current position along y-axis of room
double Xtarget  = 10;               // target position along x-axis of room
double Ytarget  = 10;               // target position along y-axis of room
double lastPosX = curPosX;          // position at previous cycle along x-axis
double lastPosY = curPosY;          // position at previous cycle along y-axis

double pgain    = 0.05;             // P-gain of the PID-controller, init = 0.05
double igain    = 0.05;             // I-gain of the PID-controller, init = 0.05
double dgain    = 0.2;              // D-gain of the PID-controller, init = 0.2
double iState   = 0;                // parameter for I-gain in PID-controller

double time     = 0;                // current time
double prevTime = -1;               // timestamp of previous cycle

double yawDeg   = 0;                // yaw of the drone in DEGREES, 0 = positive x-axis, anti-clockwise
double yaw = yawDeg/180 * PI;       // yaw of the drone to radians
double velo     = 0;                // velocity of drone, absolute value, dependent on pitch and roll angles
double veloAng  = 0;                // angle of the velocity in the room-frame, 0 = positive x-axis, anti-clockwise
int maxPitch    = 2;                // max pitch angle in DEGREES
int maxRoll     = 2;                // max roll angle in DEGREES


// ------ Loop Start ------
    double deltaTime = time - prevTime;                         // time since previous cycle
    
    // @TODO: get actual curPosX and curPosY from Pozyx + ZED
    // @TODO: if Kalman filter is implemented, use calculated positions bellow
    double curPosX = curPosX + velo * cos(veloAng) * deltaTime; // calculated current position along x-axis
    double curPosY = curPosY + velo * sin(veloAng) * deltaTime; // calculated current position along y-axis

    
    double Xerror = Xtarget - curPosX;                          // distance error along x-axis
    double Yerror = Ytarget - curPosY;                          // distance error along y-axis
    
    double errorDist = sqrt(Xerror^2 + Yerror^2);               // distance from current position to target position
    double errorAng = atan2(Yerror,Xerror);                     // angle of desired trajectory from x-axis anti-clockwise
    
    double pTerm = pgain * errorDist;                           // P-term of PID

    double iState = iState + (errorDist * deltaTime);      

    // Windup guard so that I-term does not grow infinitely
    // @TODO: Better Windup guards probably available and worth implementing
    if (iState > 5) {
        iState = 5;
    } else if (iState < -5) {
        iState = -5;
    }

    double iTerm = igain * iState;                              // I-term of PID

    double dTerm = (dgain * (sqrt(curPosX^2 + curPosY^2)
        - sqrt(lastPosX^2 + lastPosY^2))) / deltaTime;          // D-term of PID

    double posCmd = pTerm + iTerm + dTerm;                      // needed movement to reach target

    double posCmdX = posCmd * cos(errorAng);                    // needed movement along x-axis
    double posCmdY = posCmd * sin(errorAng);                    // needed movement along y-axis

    double pitchCmd = cos(yaw)*posCmdX + sin(yaw)*posCmdY;      // needed pitch of the drone
    double rollCmd = sin(yaw)*posCmdX - cos(yaw)*posCmdY;       // needed roll of the drone

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
    
    double veloX = cos(yaw)*pitchCmd + sin(yaw)*rollCmd;        // velocity along x-axis of room
    double veloY = sin(yaw)*pitchCmd - cos(yaw)*rollCmd;        // velocity along y-axis of room
    
    double velo = sqrt(veloX^2 + veloY^2);                      // total absolute velocity towards target
    double veloAng = atan2(veloY,veloX);                        // angle of the velocity from x-axis of room anti-clockwise
    
    lastPosX = curPosX;
    lastPosY = curPosY;

    // @TODO: link to computer time
    prevTime = time;
    time += time;

// ------ Loop End ------






