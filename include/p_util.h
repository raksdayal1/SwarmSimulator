/*
 * Created by Rakshit Allamraju
 * Date: Feb. 25 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 5 2022
 */

#ifndef P_UTIL_H
#define P_UTIL_H

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

void thread_sleep(double seconds);

// Vehicle definitions
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

    struct Attitude_Quat{
        double w;
        double x;
        double y;
        double z;
    }att_q;


    struct AngularRates{
        // Angular velocities measured in body frames
        double p;
        double q;
        double r;
    }ang_vel;

};

//msg packet to send over socket to Gazebo
struct gazebo_packet {
    double x_pos;
    double y_pos;
    double z_pos;
    double roll;
    double pitch;
    double yaw;
};

//

#endif // P_UTIL_H
