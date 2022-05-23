#include <ignition/math/Pose3.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"

namespace gazebo
{
class WPT : public WorldPlugin
{
  public: void Load(physics::WorldPtr _parent, sdf::ElementPtr /*_sdf*/)
  {
    
    {
      
      transport::NodePtr node(new transport::Node());

      
      node->Init(_parent->Name());
      std::cout << _parent->Name() << std::endl;
     
      transport::PublisherPtr WPTPub = 
      node->Advertise<msgs::Factory>("~/worldPluginTutorial");

      
      msgs::Factory msg;

      
      msg.set_sdf_filename("model://iris_with_standoffs");

      
      msgs::Set(msg.mutable_pose(),
          ignition::math::Pose3d(
            ignition::math::Vector3d(1, -2, 0),
            ignition::math::Quaterniond(0, 0, 0)));

     
      WPTPub->Publish(msg);
    }
  }
};


GZ_REGISTER_WORLD_PLUGIN(WPT)
}




/*MODEL SUDO CODE STUFF (Very Cool)

 void InstatiateDrone(name, posx, posy, posz, IP address) {

} */

 //void DroneInit(string name, int posx, int posy, int posz) {
     /*  { transport::NodePtr node(new transport::Node());
      node->Init(_parent->Name());
      transport::PublisherPtr worldPub = node->Advertise<msgs::WPT>("~/worldPluginTest");
      msgs::WPT msg;
      msg.set_sdf_filename("model://iris_with_standoffs");
      msgs::Set(msg.mutable_pose(), ignition::math::Pose3d(ignition::math::Vector3d(1, -2, 0), ignition::math::Quaterniond(0, 0, 0)));
      worldPub->Publish(msg); <-- attempted message system - poor results
       } */
   // }


    //_parent->gazebo::physics::World::EnablePhysicsEngine(false); <-- Most likely the closest to being correct
    /*physics::Model(_parent->InsertModelFile("model://iris_with_standoffs")) //constructor
    physics::~Model() */ // <-- MOST LIKELY INCORRECT
    //physics::Model Init()
    /*physics::ModelPtr coolDrone = _parent->InsertModelFile("model://iris_with_standoffs");
    coolDrone->SetWorldPose(0, 0, 3); */ //<-- BAD CODE
 //EnablePhysicsEngine(false);
 /* public: void OnUpdate() 
    {
      this->enablePhysicsEngine = false;
    } */

/*sdf::SDF DroneMod;
    DroneMod.SetFromString(
       "<sdf version ='1.4'>\
          <model name ='sphere'>\
            <pose>1 0 0 0 0 0</pose>\
            <link name ='link'>\
              <pose>0 0 .5 0 0 0</pose>\
              <collision name ='collision'>\
                <geometry>\
                  <sphere><radius>0.5</radius></sphere>\
                </geometry>\
              </collision>\
              <visual name ='visual'>\
                <geometry>\
                  <sphere><radius>0.5</radius></sphere>\
                </geometry>\
              </visual>\
            </link>\
          </model>\
        </sdf>");
    // Demonstrate using a custom model name.
    sdf::ElementPtr model = DroneMod.Root()->GetElement("model");
    model->GetAttribute("name")->SetFromString("unique_Drone");
    _parent->InsertModelSDF(DroneMod); <--- WILL NOT WORK; Needs model within the world plugin. */
   
   //#include "gazebo/physics/World.hh"