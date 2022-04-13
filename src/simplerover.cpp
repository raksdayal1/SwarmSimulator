/*
 * Created by Rakshit Allamraju
 * Date: Feb. 24 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 5 2022
 */
#include "simplerover.h"
#include "p_util.h"

uint16_t servo_out[16];

struct gazebo_packet {
    double x_pos;
    double y_pos;
    double z_pos;
    double roll;
    double pitch;
    double yaw;
} gpkt_r;


SimpleRover::SimpleRover(double x, double y, double theta,  uint16_t _ap_port, uint16_t _gazebo_port)
{

    this->x = x;
    this->y = y;
    this->theta = theta;

    this->json = new libAP_JSON();

    this->gazebo_port = _gazebo_port;
    this->ap_port = _ap_port;

    if(this->json->InitSockets("127.0.0.1", this->ap_port)){
        std::cout<<"Started socket on 127.0.0.1:" << this->ap_port << std::endl;
    }

    addIntegrator(this->x, this->vx);
    addIntegrator(this->y, this->vy);
    addIntegrator(this->theta, this->theta_dot);

}

void SimpleRover::update()
{

    if( State::sample( 1.0/400) || State::tickfirst || State::ticklast) {
      this->state.timestamp = State::t;
      //printf( "%12s t= %8.3f, x = %8.3f, vx = %8.3f\n", "Balll", this->state.timestamp, this->old_state.timestamp, vx);

        /*
        double timestep = this->state.timestamp - this->old_state.timestamp;
        if (timestep < 0) {
            // the sim is trying to go backwards in time
            std::cout << "[simpleRover] Error: Time went backwards" << std::endl;

        } else if (timestep == 0) {
            // time did not advance. no physics step
            std::cout << "[simpleRover] Warning: Time did not step forward" << std::endl;

        } else if (timestep > 60) {
            // limiting timestep to less than 1 minute
            std::cout << "[simpleRover] Warning: Time step was very large" << std::endl;
            thread_sleep(timestep);

        }
        */


    if (this->json->ReceiveServoPacket(servo_out))
    {
        //printf( "t= %8.3f, x= %8.3f, y= %8.3f, theta= %8.3f, s0= %8.3d, s2= %8.3d\n",
        //        this->state.timestamp, this->x, this->y, this->theta, servo_out[0], servo_out[2]);

        // how fast is the rover moving
        double max_velocity = 10

                ; // m/s
        double body_v = _interp1D(servo_out[2], 1100, 1900, -max_velocity, max_velocity);

        double max_turn_rate = 15 * M_PI / 180;
        double body_omega_z = _interp1D(servo_out[0], 1100, 1900, -max_turn_rate, max_turn_rate);

        this->V = body_v;

        this->vx = this->V*cos(this->theta);
        this->vy = this->V*sin(this->theta);
        this->theta_dot = body_omega_z;

        //send data to Gazebo
        gpkt_r.x_pos = this->x;
        gpkt_r.y_pos = this->y;
        gpkt_r.z_pos = 0.1;

        gpkt_r.pitch = 0.0;
        gpkt_r.roll = 0.0;
        gpkt_r.yaw = this->theta;

        auto bytes_sent = gazebo_sock.sendto(&gpkt_r, sizeof(gpkt_r), "127.0.0.1", this->gazebo_port);

        std::cout << "sending " << bytes_sent << " bytes to gazebo" <<std::endl;


        this->json->SendState(this->state.timestamp,
                     0, 0, this->theta_dot,    // gyro
                     0, 0, -9.81, // accel
                     this->x, this->y, 0,    // position
                     0, 0, this->theta,    // attitude
                     this->vx, this->vy, 0);    // velocity

    }

    }
    this->old_state.timestamp = this->state.timestamp;
}

void SimpleRover::rpt()
{
    //if( State::sample( 1.0) || State::tickfirst || State::ticklast) {
    //  printf( "%12s t= %8.3f, x = %8.3f, vx = %8.3f\n", "Balll", State::t, x, vx);
    //}


}

double SimpleRover::_interp1D(const double &x, const double &x0, const double &x1, const double &y0, const double &y1)
{
    return ((y0 * (x1 - x)) + (y1 * (x - x0))) / (x1 - x0);
}

