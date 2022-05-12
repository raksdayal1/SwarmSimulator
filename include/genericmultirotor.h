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
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include "libAP_JSON.h"
#include "SocketExample.h"
#include "sim.h"
#include <p_util.h>

class GenericMultirotor : public Block
{
public:
    GenericMultirotor(double x, double y, double z,
                      double phi, double theta, double psi, uint16_t _ap_port, uint16_t _gazebo_port);

    void update();
    void rpt();

private:
    // State information
    VehicleState rotorState;
    double x_dot, y_dot, z_dot, u_dot, v_dot, w_dot;
    double phi_dot, theta_dot, psi_dot, p_dot, q_dot, r_dot;
    double x, y, z, phi, theta, psi;

    // Servo values
    uint16_t servo_out[16];

    // connections to ardupilot and Gazebo
    libAP_JSON *json;

    SocketExample gazebo_sock = SocketExample(true);
    uint16_t gazebo_port, ap_port;

    //Inertial parameters
    double mass;
    Eigen::Matrix3d Inertia_matrix;

    // Private functions for Vehicle Dynamics and Interactions
    double GroundEffect(Eigen::Vector3d pos);
    void PropellerPhysics(); // This is similar to LiftDrag Plugin in Gazebo
    void ApplyController(); // This is similar to Ardupilot Plugin in Gazebo

    // Private math functions
    double _interp1D(const double &x, const double &x0, const double &x1, const double &y0, const double &y1);
    double _limit(double value, double lim);
    Eigen::Vector3d _limitVec(Eigen::Vector3d vec, Eigen::Vector3d lims);

};

#endif // GENERICMULTIROTOR_H
