/*
 * Created by Rakshit Allamraju
 * Date: Feb. 25 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 5 2022
 */
#include "genericmultirotor.h"

#define GRAVITY_ON 1
#define DEBUG 1

gazebo_packet gpkt;

GenericMultirotor::GenericMultirotor(double x, double y, double z,
                                     double phi, double theta, double psi, uint16_t _ap_port, uint16_t _gazebo_port)
{

    this->json = new libAP_JSON();

    this->gazebo_port = _gazebo_port;
    this->ap_port = _ap_port;

    if(this->json->InitSockets("127.0.0.1", this->ap_port)){
        std::cout<<"[Multirotor]: Started socket on 127.0.0.1:" << this->ap_port << std::endl;
    }

    this->x = x;
    this->y = y;
    this->z = z;
    this->phi = phi;
    this->theta = theta;
    this->psi = psi;

    // Position integrators
    addIntegrator(this->rotorState.pos.x, this->x_dot);
    addIntegrator(this->rotorState.pos.y, this->y_dot);
    addIntegrator(this->rotorState.pos.z, this->z_dot);

    // Velocity integrators
    addIntegrator(this->rotorState.vel.u, this->u_dot);
    addIntegrator(this->rotorState.vel.v, this->v_dot);
    addIntegrator(this->rotorState.vel.w, this->w_dot);

    // Attitude integrators
    addIntegrator(this->rotorState.att.phi, this->phi_dot);
    addIntegrator(this->rotorState.att.theta, this->theta_dot);
    addIntegrator(this->rotorState.att.psi, this->psi_dot);

    // Angular velocities integrators
    addIntegrator(this->rotorState.ang_vel.p, this->p_dot);
    addIntegrator(this->rotorState.ang_vel.q, this->q_dot);
    addIntegrator(this->rotorState.ang_vel.r, this->r_dot);

    // Just test values.... remove later
    this->mass = 1;
    this->Inertia_matrix(0,0) = 2.0*this->mass*pow(0.005,2)/5 + 2*pow(0.5,2)*0.01; this->Inertia_matrix(0,1) = 0; this->Inertia_matrix(0,2) = 0;
    this->Inertia_matrix(1,0) = 0; this->Inertia_matrix(1,1) = 2.0*this->mass*pow(0.005,2)/5 + 2*pow(0.5,2)*0.01;; this->Inertia_matrix(1,2) = 0;
    this->Inertia_matrix(2,0) = 0; this->Inertia_matrix(2,1) = 0; this->Inertia_matrix(2,2) = 2.0*this->mass*pow(0.005,2)/5 + 4*pow(0.5,2)*0.01;
}


void GenericMultirotor::update()
{

    double g(9.81);
    if( State::sample( 1.0/1000) || State::tickfirst || State::ticklast) {
      this->rotorState.timestamp = State::t;

        //if(DEBUG){
        //    printf( "t= %8.3f, x = %8.3f, y = %8.3f, z = %8.3f\n",
        //    this->rotorState.timestamp, this->rotorState.pos.x, this->rotorState.pos.y, this->rotorState.pos.z);
       // }

        if (this->json->ReceiveServoPacket(this->servo_out))
        {

            if(DEBUG){
                std::cout <<"Servos = " << this->servo_out[0] << ", " << this->servo_out[1]
                     << ", " << this->servo_out[2] << ", " << this->servo_out[3] <<"\n";
            }

            Eigen::Matrix3d R_vb_matrix, T_matrix; // R_vb relates body frame velocity to inertial frame vel, T relates angular rates
            Eigen::Vector3d ang_vel_vector, vel_vector, torque_vector, force_vector, gravity(0,0,0), normal(0,0,0);
            Eigen::Vector3d ang_accel_out, accel_out, ang_vel_out, vel_out;
            Eigen::Matrix4d Mixing;
            Eigen::Vector4d servo_out_norm;

            // Limit the state values if they exceed physical limits
            this->rotorState.vel.u = _limit(this->rotorState.vel.u, 10);
            this->rotorState.vel.v = _limit(this->rotorState.vel.v, 10);
            this->rotorState.vel.w = _limit(this->rotorState.vel.w, 10);

            if (GRAVITY_ON){

                // Using transformed values now... but need to generalize in the future
                gravity(0) = -g*sin(this->rotorState.att.theta);
                gravity(1) = g*cos(this->rotorState.att.theta)*sin(this->rotorState.att.phi);
                gravity(2) = g*cos(this->rotorState.att.theta)*cos(this->rotorState.att.phi);
            }
            else{
                gravity(0) = 0;
                gravity(1) = 0;
                gravity(2) = 0;
            }

            Eigen::Vector3d curr_pos;
            curr_pos(0) = this->rotorState.pos.x;
            curr_pos(1) = this->rotorState.pos.y;
            curr_pos(2) = this->rotorState.pos.z;

            double point_ground_effect = GroundEffect(curr_pos);
            if(point_ground_effect > 0)
            {
                // need to vehicle position to 1 m above ground

                if(accel_out(2) < 0){
                    this->rotorState.vel.u = 0;
                    this->rotorState.vel.v = 0;
                    this->rotorState.vel.w = 0;
                }

                // Add normal forces
                normal(0) = 0;
                normal(1) = 0;
                normal(2) = g*cos(this->rotorState.att.theta)*cos(this->rotorState.att.phi);
            }
            else if(point_ground_effect < 0)
            {
                // Zero out normal force
                normal(0) = 0; normal(1) = 0; normal(2) = 0;
            }


            // R_vb matrix
            R_vb_matrix(0,0) = cos(this->rotorState.att.theta)*cos(this->rotorState.att.psi);
            R_vb_matrix(0,1) = sin(this->rotorState.att.phi)*sin(this->rotorState.att.theta)*cos(this->rotorState.att.psi) -
                                cos(this->rotorState.att.theta)*sin(this->rotorState.att.psi);
            R_vb_matrix(0,2) = cos(this->rotorState.att.phi)*sin(this->rotorState.att.theta)*cos(this->rotorState.att.psi) +
                    sin(this->rotorState.att.theta)*sin(this->rotorState.att.psi);

            R_vb_matrix(1,0) = cos(this->rotorState.att.theta)*sin(this->rotorState.att.psi);
            R_vb_matrix(1,1) = sin(this->rotorState.att.phi)*sin(this->rotorState.att.theta)*sin(this->rotorState.att.psi) +
                                cos(this->rotorState.att.theta)*cos(this->rotorState.att.psi);
            R_vb_matrix(1,2) = cos(this->rotorState.att.phi)*sin(this->rotorState.att.theta)*sin(this->rotorState.att.psi) -
                    sin(this->rotorState.att.theta)*cos(this->rotorState.att.psi);

            R_vb_matrix(2,0) = sin(this->rotorState.att.theta);
            R_vb_matrix(2,1) = -sin(this->rotorState.att.phi)*cos(this->rotorState.att.theta);
            R_vb_matrix(2,2) = -cos(this->rotorState.att.phi)*cos(this->rotorState.att.theta);

            if(DEBUG){
                std::cout << R_vb_matrix << std::endl;
            }

            // END R_vb matrix

            // T_matrix
            T_matrix(0,0) = 1; T_matrix(0,1) = sin(this->rotorState.att.phi)*tan(this->rotorState.att.theta); T_matrix(0,2) = cos(this->rotorState.att.phi)*tan(this->rotorState.att.theta);
            T_matrix(1,0) = 0; T_matrix(1,1) = cos(this->rotorState.att.phi); T_matrix(1,2) = -sin(this->rotorState.att.phi);
            T_matrix(2,0) = 0; T_matrix(2,1) = sin(this->rotorState.att.phi)/cos(this->rotorState.att.theta); T_matrix(2,2) = cos(this->rotorState.att.phi)/cos(this->rotorState.att.theta);
            // END T_matrix

            // Torque and Force vector
            for (int p=0;p<4;p++){
                servo_out_norm(p) = _interp1D(servo_out[p], 1000, 1900, 0, 40);
            }

            if(DEBUG){
                std::cout <<"Servos = " << servo_out[0] << ", " << servo_out[1]
                     << ", " << servo_out[2] << ", " << servo_out[3] <<"\n";
                std::cout <<"Servos normed = " << servo_out_norm(0) << ", " << servo_out_norm(1)
                     << ", " << servo_out_norm(2) << ", " << servo_out_norm(3) <<"\n";
            }

            Mixing << 0.1,0.1,0.1,0.1,
                    0,-0.05,0,0.05,
                    0.05,0,-0.05,0,
                    -0.1,0.1,-0.1,0.1;

            Eigen::Vector4d Forces;
            Forces = Mixing*servo_out_norm;

            if(DEBUG){
                std::cout <<"Force = " << Forces(0) << ", " << Forces(1)
                     << ", " << Forces(2) << ", " << Forces(3) <<"\n";
            }

            torque_vector(0) = 0;//Forces(1);
            torque_vector(1) = 0;//Forces(2);
            torque_vector(2) = 0;//Forces(3);
            force_vector(0) = 0; force_vector(1) = 0; force_vector(2) = Forces(0);

            // Velocity and angular velocity vector
            vel_vector(0) = this->rotorState.vel.u;
            vel_vector(1) = this->rotorState.vel.v;
            vel_vector(2) = this->rotorState.vel.w;
            ang_vel_vector(0) = this->rotorState.ang_vel.p;
            ang_vel_vector(1) = this->rotorState.ang_vel.q;
            ang_vel_vector(2) = this->rotorState.ang_vel.r;

            ang_accel_out = this->Inertia_matrix.inverse()*( torque_vector - ang_vel_vector.cross(this->Inertia_matrix*ang_vel_vector));
            //accel_out = _limitVec((1.0/this->mass)*force_vector - ang_vel_vector.cross(vel_vector) - gravity,
            //                   Eigen::Vector3d(2*g, 2*g, 2*g));// + normal;
            accel_out = (1.0/this->mass)*force_vector - ang_vel_vector.cross(vel_vector) - gravity + normal;
            ang_vel_out = T_matrix*ang_vel_vector;

            //vel_out = _limitVec(R_vb_matrix*vel_vector, Eigen::Vector3d(10,10,3));
            vel_out = R_vb_matrix*vel_vector;

            //std::cout <<"gravity + normal = " << gravity(0) + normal(0) << ", " << gravity(1) + normal(1)
            //         << ", " << gravity(2) + normal(2) <<"\n";

            this->p_dot = ang_accel_out(0); this->q_dot = ang_accel_out(1); this->r_dot = ang_accel_out(2);
            this->u_dot = accel_out(0); this->v_dot = accel_out(1); this->w_dot = accel_out(2);

            this->phi_dot = ang_vel_out(0); this->theta_dot = ang_vel_out(1); this->psi_dot = ang_vel_out(2);
            this->x_dot = vel_out(0); this->y_dot = vel_out(1); this->z_dot = vel_out(2);

            if(DEBUG){
                printf( "t= %8.3f, x = %8.3f, y = %8.3f, z = %8.3f\n",
                            this->rotorState.timestamp, this->rotorState.pos.x, this->rotorState.pos.y, this->rotorState.pos.z);
                std::cout <<"velocity vector = " << vel_vector(0) << ", " << vel_vector(1)
                         << ", " << vel_vector(2) <<"\n";
                std::cout <<"accel = " << accel_out(0) << ", " << accel_out(1)
                         << ", " << accel_out(2) <<"\n";
                std::cout <<"angles = " << this->rotorState.att.phi << ", " << this->rotorState.att.theta
                         << ", " << this->rotorState.att.psi <<"\n";
                std::cout <<"vel = " << vel_out(0) << ", " << vel_out(1)
                         << ", " << vel_out(2) <<"\n";
                std::cout << "-----------------------------------------------------" << std::endl;
            }

            //send data to Gazebo
            gpkt.x_pos = this->rotorState.pos.x;
            gpkt.y_pos = this->rotorState.pos.y;
            gpkt.z_pos = -1*this->rotorState.pos.z;

            gpkt.pitch = gpkt.pitch;
            gpkt.roll = gpkt.roll;
            gpkt.yaw = gpkt.yaw;

            auto bytes_sent = gazebo_sock.sendto(&gpkt, sizeof(gpkt), "127.0.0.1", this->gazebo_port);

            if(DEBUG){
                std::cout << "sending " << bytes_sent << " bytes to gazebo" <<std::endl;
            }

            // Send fdm integrated states to Pixhawk
            this->json->SendState(this->rotorState.timestamp,
                         this->rotorState.ang_vel.p, this->rotorState.ang_vel.q, this->rotorState.ang_vel.r,    // gyro
                         accel_out(0), accel_out(1), accel_out(2), // accel
                         this->rotorState.pos.x, this->rotorState.pos.y, -1*this->rotorState.pos.z,    // position
                         this->rotorState.att.phi, this->rotorState.att.theta, this->rotorState.att.psi,    // attitude
                         this->rotorState.vel.u, this->rotorState.vel.v, this->rotorState.vel.w);    // velocity

        }

    }

}

void GenericMultirotor::rpt()
{
    // Pass
}

void GenericMultirotor::PropellerPhysics(){

}

void GenericMultirotor::ApplyController(){

}

double GenericMultirotor::GroundEffect(Eigen::Vector3d pos){
    Eigen::Vector4d plane, pos_extended;

    // to check if point lies on the plane, or either of the half surfaces created by the plane
    // take the dot product dot( [a,b,c,d], [x,y,z,1] ), where aX+bY+cZ+d = 0 is the equation of the plane
    // and (x,y,z) is the point.

    // Ground plane equation is 0X+0Y+1Z+0 = 0 i.e. Z=0
    plane(0) = 0; plane(1) = 0; plane(2) = 1; plane(3) = 0;
    pos_extended(0) = pos(0); pos_extended(1) = pos(1); pos_extended(2) = pos(2); pos_extended(3) = 1;

    double out = pos_extended.dot(plane);

    return out;
}

double GenericMultirotor::_interp1D(const double &x, const double &x0, const double &x1, const double &y0, const double &y1)
{
    return ((y0 * (x1 - x)) + (y1 * (x - x0))) / (x1 - x0);
}

double GenericMultirotor::_limit(double value, double lim){
    double out;
    if(value > lim){
        out = lim;
    }else if(value < -lim){
        out = -lim;
    }else{
         out = value;
    }

    return out;
}

Eigen::Vector3d GenericMultirotor::_limitVec(Eigen::Vector3d vec, Eigen::Vector3d lims){

    Eigen::Vector3d out;

    if(vec(0) > lims(0)){
        out(0) = lims(0);
    }else if(vec(0) < -lims(0)){
        out(0) = -lims(0);
    }else{
         out(0) = vec(0);
    }

    if(vec(1) > lims(1)){
        out(1) = lims(1);
    }else if(vec(1) < -lims(1)){
        out(1) = -lims(1);
    }else{
         out(1) = vec(1);
    }

    if(vec(2) > lims(2)){
        out(2) = lims(2);
    }else if(vec(2) < -lims(2)){
        out(2) = -lims(2);
    }else{
         out(2) = vec(2);
    }

    return out;
}
