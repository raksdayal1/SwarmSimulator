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
            /*
            if (_sdf->HasElement("link"))
            {
                    sdf::ElementPtr elem = _sdf->GetElement("link");

                    this->linkName = elem->Get<std::string>();
                    this->link = this->model->GetLink(this->linkName);

            }
            else{
                    ROS_INFO("[Plugin Error]: No depthcam link found");
            }

            if (_sdf->HasElement("gimbaljoint"))
            {
                    sdf::ElementPtr elem = _sdf->GetElement("gimbaljoint");

                    this->jointName = elem->Get<std::string>();
                    this->joint = this->model->GetJoint(this->jointName);

            }
            else{
                    ROS_INFO("[Plugin Error]: No joint found");
            }
            */

            // set model to static to make it unaffected by physics engine (DOES THIS WORK???)
            this->model->SetStatic(true);

            const char *gazebo_address;
            const uint16_t gazebo_port_in(5006); // random port assignment and hardcoded

            gazebo_address = "127.0.0.1";

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

            std::cout << "CMD Link module ready" <<std::endl;

        };

        virtual void Init(){

            this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                    boost::bind(&CMDPlugin::OnUpdate, this));
        };

    private:

        virtual void OnUpdate(){

            auto recvSize = sock.recv(&pkt, sizeof(pkt), 1);

            std::cout << pkt.x_pos << ", " << pkt.y_pos << ", " << pkt.z_pos << ", " <<
                         pkt.roll << ", "<<pkt.pitch << ", " << pkt.yaw << std::endl;

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
