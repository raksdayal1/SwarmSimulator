#ifndef LIBAP_JSON_H
#define LIBAP_JSON_H

#include <iostream>
#include <string>

#include "SocketExample.h"

class libAP_JSON {
public:
    bool InitSockets(const char *fdm_address, const uint16_t fdm_port_in);
    bool ReceiveServoPacket(uint16_t servo_out[]);
    void SendState(double timestamp,
                   double gyro_x, double gyro_y, double gyro_z, // rad/sec
                   double accel_x, double accel_y, double accel_z, // m/s^2
                   double pos_x, double pos_y, double pos_z, // m in inertial frame
                   double phi, double theta, double psi, // attitude radians
                   double V_x, double V_y, double V_z); // m/s in inertial frame
    void setAirspeed(double airspeed_in); // m/s
    void setWindvane(double direction, // radians clockwise to the front (0 is head to wind)
                     double speed); // m/s
    void setRangefinder(double *rangefinder_in, uint8_t n);
    bool ap_online;
private:
    // Socket manager
    SocketExample sock = SocketExample(true);

    // The address for the flight dynamics model
    // const char *libAP_JSON::fdm_address;

    // The address for the SITL flight controller - auto detected
    const char *fcu_address;

    // The port for the flight dynamics model
    // uint16_t libAP_JSON::fdm_port_in;

    // The port for the SITL flight controller - auto detected
    uint16_t fcu_port_out;

    // Number of consecutive missed ArduPilot controller messages
    int connectionTimeoutCount;

    // Max number of consecutive missed ArduPilot controller messages before timeout
    int connectionTimeoutMaxCount = 10;

    // Last received frame rate from the ArduPilot controller
    uint16_t fcu_frame_rate;

    // Last received frame count from the ArduPilot controller
    uint32_t fcu_frame_count = -1;

    // optional JSON data
    double airspeed;
    bool set_airspeed_flag = false;
    double windvane_direction;
    double windvane_speed;
    bool set_windvane_flag = false;
    double rangefinder[6];
    uint8_t rangefinder_count = 0;
};

#endif
