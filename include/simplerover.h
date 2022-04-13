/*
 * Created by Rakshit Allamraju
 * Date: Feb. 24 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 13 2022
 */

#ifndef SIMPLEROVER_H
#define SIMPLEROVER_H

#include <math.h>
#include <stdlib.h>
#include "libAP_JSON.h"

#include "sim.h"

class SimpleRover : public Block
{
public:
    SimpleRover(double x, double y, double theta, uint16_t _ap_port, uint16_t _gazebo_port);
    void update();
    void rpt();

    struct SimpleRoverState {
        double timestamp = 0;
        double gyro_x, gyro_y, gyro_z = 0; // rad/sec
        double accel_x, accel_y, accel_z = 0; // m/s^2
        double pos_x, pos_y, pos_z = 0; // m in inertial frame
        double phi, theta, psi = 0; // attitude radians
        double V_x, V_y, V_z = 0; // m/s in inertial frame
    } state, old_state;

private:
    double x, y, theta, theta_dot;
    double vx, vy, V;
    libAP_JSON *json;

    uint16_t gazebo_port, ap_port;

    //connection to Gazebo
    SocketExample gazebo_sock = SocketExample(true);
    //gazebo_packet gpkt;


    double _interp1D(const double &x, const double &x0, const double &x1, const double &y0, const double &y1);
};

#endif // SIMPLEROVER_H
