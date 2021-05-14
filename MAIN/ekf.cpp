#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

class Kalman_Filter_Pos {
    protected:
    double              A_k [2][2] = {{1.0, 0.0}, {0.0, 1.0}};    //A matrix, express how the states x and y changes
    double              V_k [2] = {0.01, 0.01};                   //noise applied to the forward kinematics
    double              Q_k [2][2] = {{1.0, 0}, {0, 1.0}};        //state model noise covariance
    double              H_k [2][2] = {{1.0, 0}, {0, 1.0}};        //convert the predicted sensor measurements at time k
    double              R_k [2][2] = {{1.0, 0}, {0, 1.0}};        //sensor measurement noise covariance
    double              w_k [2] = {0.07, 0.07};                   //sensor measurement
    double              u_k [2] = {4.5, 0};                       //control vector
    double              X_k [2] = {0, 0};                         //state estimate
    double              P_K [2][2] = {{0.01, 0}, {0, 0.01}};      //initial error covariance
    double              Y_k;                                      //measurement residual
    double              S_k;                                      //measurement residual covariance
    double              K_k;                                      //initial Kalman gain
    double              Z_k [2]= {lastPosX, lastPosY};            //initial Kalman gain


    public:
    Kalman_Filter_Pos(double z_k);

    //##### prediction ########

    //state estimate
    X_k = (A_k * X_k) + u_k + V_k;

    // covariance of the state
    P_k = (A_k * P_k * A_k) + Q_k;

    //#######  measurement (upodate) ########

    // Y_k residual measurement
    Y_k =  Z_k - ((H_k * X_k) + w_k);

    // residual covariance
    S_k = (H_k * P_K * H_k) + R_k;

    // Kalman gain
    K_X = (P_k * H_k) / S_k;

    // updated state estimate
    X_k = X_k + (K_k * Y_k);

    // updated covariance od the state
    P_X = P_k - (K_k * H_k * P_k);

    // Returns updated value for Pos_x and Pos_y
    reurtn [X_k[0], X_k[1]];
