/*
 * Created by Rakshit Allamraju
 * Date: March 3 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 5 2022
 */
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <thread>
#include <map>
#include <math.h>

#include "gazebo/common/Assert.hh"
//#include "gazebo/transport/transport.hh"
#include "gazebo/common/Plugin.hh"
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"

#include <ignition/math/Vector3.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Quaternion.hh>

#include "SocketExample.h"

#define MAXLEN 1024

using namespace ignition::math;

namespace gazebo
{
    class CMDPlugin : public ModelPlugin
    {
    public:
        CMDPlugin(){
        };

        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf){


            GZ_ASSERT(_model, "TestDynamics _model pointer is NULL");
            this->model = _model; // model pointer
            this->modelName = _model->GetName();

            this->world = this->model->GetWorld();
            GZ_ASSERT(this->world, "TestDynamics world pointer is NULL");

            this->physics = this->world->Physics();
            GZ_ASSERT(this->physics, "TestDynamics physics pointer is NULL");

            char *gazebo_address; // Check if reassignment of this variable is done correctly.
            uint16_t gazebo_port_in;

            // Get the IP address from SDF
            if (_sdf->HasElement("IPAddress"))
            {
                sdf::ElementPtr elem = _sdf->GetElement("IPAddress");

                std::string ip_addr =  elem->Get<std::string>();

                gazebo_address = (char *)alloca(ip_addr.size() + 1);
                memcpy(gazebo_address, ip_addr.c_str(), ip_addr.size() + 1);

                std::cout << "[" << this->modelName << "]: "
                          <<"Setting to IPAddress: " << gazebo_address << std::endl;

            }
            else{
                std::cout << "[" << this->modelName << "]: "
                          <<"IPAddress not set. reverting to default 127.0.0.1 "<< std::endl;
            }

            // Get the IP address from SDF
            if (_sdf->HasElement("CMDPort"))
            {
                sdf::ElementPtr elem = _sdf->GetElement("CMDPort");

                std::string port_in =  elem->Get<std::string>();

                gazebo_port_in = stoi(port_in);

                std::cout << "[" << this->modelName << "]: "
                          <<"Setting to port: " << gazebo_port_in << std::endl;

            }
            else{
                std::cout << "[" << this->modelName << "]: "
                          <<"CMDPort not set. exiting the plugin and gazebo application "<< std::endl;
                exit(-1);
            }


            // bind the socket
            if (!sock.bind(gazebo_address, gazebo_port_in))
            {
                std::cout << "Failed to bind with "
                          << gazebo_address << ":" << gazebo_port_in << std::endl;

            }
            else{
                std::cout << "Gazebo model at "
                          << gazebo_address << ":" << gazebo_port_in << std::endl;
            }


            // Get the Initial position from SDF
            if (_sdf->HasElement("InitialPosition"))
            {
                sdf::ElementPtr elem = _sdf->GetElement("InitialPosition");

                std::string init_pos =  elem->Get<std::string>();

                std::vector<std::string> pos_array;

                size_t start;
                size_t end = 0;

                while ((start = init_pos.find_first_not_of(" " , end)) != std::string::npos)
                {
                    end = init_pos.find(" ", start);
                    pos_array.push_back(init_pos.substr(start, end - start));
                }

                // parsing 0 to 5 elements in string after splitting on delimiter
                // and setting to initial position packet
                pkt.x_pos = std::stod(pos_array.at(0));
                pkt.y_pos = std::stod(pos_array.at(1));
                pkt.z_pos = std::stod(pos_array.at(2));
                pkt.roll = std::stod(pos_array.at(3));
                pkt.pitch = std::stod(pos_array.at(4));
                pkt.yaw = std::stod(pos_array.at(5));

            }
            else{
                // Set the initial position to 0
                pkt.x_pos = 0;
                pkt.y_pos = 0;
                pkt.z_pos = 0;
                pkt.roll = 0;
                pkt.pitch = 0;
                pkt.yaw = 0;

                std::cout << "[" << this->modelName << "]: "
                          <<"Setting initial position of model to (0,0,0,0,0,0). This might cause errors. "<< std::endl;
            }

            // set model to static to make it unaffected by physics engine (DOES THIS WORK???)
            this->model->SetStatic(true);

            std::cout << "CMD Link module ready" <<std::endl;

        };

        virtual void Init(){

            this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                    boost::bind(&CMDPlugin::OnUpdate, this));
        };

    private:

        virtual void OnUpdate(){

            auto recvSize = sock.recv(&pkt, sizeof(pkt), 1);

            //std::cout << recvSize << std::endl;

            //std::cout << pkt.x_pos << ", " << pkt.y_pos << ", " << pkt.z_pos << ", " <<
            //             pkt.roll << ", "<<pkt.pitch << ", " << pkt.yaw << std::endl;

            ignition::math::Pose3d pose(pkt.x_pos, pkt.y_pos, pkt.z_pos, pkt.roll, pkt.pitch, pkt.yaw);  // = orig_pose;
            this->model->SetWorldPose(pose);

        };

        // Gazebo Variables
        std::string modelName, linkName, jointName;
        event::ConnectionPtr updateConnection;

        physics::WorldPtr world;
        physics::PhysicsEnginePtr physics;
        physics::ModelPtr model;
        physics::LinkPtr link;
        physics::JointPtr joint;
        double time_curr, prev_time, dt;

        // Socket variables
        SocketExample sock = SocketExample(true);
        char buffer[MAXLEN];

        //
        struct gazebo_packet {
            double x_pos = 0;
            double y_pos = 0;
            double z_pos = 0;
            double roll = 0;
            double pitch = 0;
            double yaw = 0;
        } pkt;


    };

    GZ_REGISTER_MODEL_PLUGIN(CMDPlugin);

}
