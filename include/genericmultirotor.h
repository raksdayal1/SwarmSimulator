/*
 * Created by Rakshit Allamraju
 * Date: Feb. 25 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 5 2022
 */

#ifndef GENERICMULTIROTOR_H
#define GENERICMULTIROTOR_H

#include <math.h>
#include <stdlib.h>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "libAP_JSON.h"
#include "SocketExample.h"
#include "sim.h"

struct VehicleState{
    double timestamp = 0;
    // Initial reference Quadrotor Dynamics and Control by Randal Beard, page 11
    struct Position{
        // Position is in inertial frame (Generally NED)
        double x; // north
        double y; // east
        double z; // down
    }pos;

    struct Velocity{
        // Body frame velocity
        double u; // velocity of vehicle projected to body x frame
        double v; // velocity of vehicle projected to body y frame
        double w; // velocity of vehicle projected to body z frame
    }vel;

    struct Attitude{
        // For right definition on frames for measurements of Euler angles
        // refer to Quadrotor Dynamics and Control by Randal Beard Rev 0.1, pages 6 to 8
        double psi; // Yaw angle
        double theta; // Pitch angle
        double phi; // Roll angle
    }att;

    struct AngularRates{
        // Angular velocities measured in body frames
        double p;
        double q;
        double r;
    }ang_vel;

};

//Move this to  different class later
double GroundEffect(Eigen::Vector3d pos);

class GenericMultirotor : public Block
{
public:
    GenericMultirotor(libAP_JSON *json);

    void update();
    void rpt();

private:
    // State information
    VehicleState rotorState;
    double x_dot, y_dot, z_dot, u_dot, v_dot, w_dot;
    double phi_dot, theta_dot, psi_dot, p_dot, q_dot, r_dot;

    // Servo values
    uint16_t servo_out[16];

    // connection to ardupilot
    libAP_JSON *json;

    //connection to Gazebo
    SocketExample gazebo_sock = SocketExample(true);
    //gazebo_packet gpkt;

    //Inertial parameters
    double mass;
    Eigen::Matrix3d Inertia_matrix;

    double _interp1D(const double &x, const double &x0, const double &x1, const double &y0, const double &y1);
    double _limit(double value, double lim);
    Eigen::Vector3d _limitVec(Eigen::Vector3d vec, Eigen::Vector3d lims);



};

#endif // GENERICMULTIROTOR_H
