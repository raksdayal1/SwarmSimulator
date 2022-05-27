# SwarmSimulator

1.	Project Structure
This project should contain the source code for OSK CMD, the simulator core and the plugins required by gazebo to visualize the vehicles. 
The simulator core contains the source code for compiling the CMD physics engine for a rover model and a quadcopter model. In the CMake file the code compiles to create an executable called PhysicsEngine. It also holds the source code for compiling a model plugin for gazebo. Gazebo plugins are linux static libraries with .so extensions and they need to be called from the .sdf file for the model placed in gazebo.The folder also contains worlds, models and plugins required by gazebo for simulating and visualizing our vehicles. 

2.	How to compile
 Make a new build folder using the mkdir command and cd into it
	$mkdir build && cd build
Run cmake
	$cmake ..
Once its completed run the make command
          $make
Once the compilation is successful, you should see an executable called PhysicsEngine and a static library called CMDPlugin.so. Copy the CMDPlugin.so to plugins folder.

3.	Running the code
3.1.	Starting a rover model
Open 3 different terminals.
In the 1st terminal cd into worlds folder and run gazebo with the following command
	$gazebo --verbose cmdrover_test.world
This will open an empty world with a single rover placed at the origin
In the 2nd terminal cd into the build folder and run the PhysicsEngine executable as follows
	$./PhysicsEngine ../init.json
The argument 0 is switching the physics engine to run a rover model
In the 3rd terminal cd into ardupilot directory and run the following command
	$sim_vehicle.py --v Rover --f JSON --map –console

3.2.	 Running the rover model
In the terminal from which you started sim_vehicle.py, once the Autopilot is ready you should be able to control the rover model in gazebo. Type in the following commands
arm uncheck all
arm throttle (you should see the arm is armed in the console as shown in figure below
 

                    

rc 3 1800 (this throttles the rover forward) 

The range is 1100 to 1900 and 1500 is zero throttle

rc 1 1600 (this controls the heading of the rover)
The range is 1100 to 1900 and 1500 is stick center

4.	Running the code
4.1.	Starting a quadcopter model
Open 3 different terminals.
In the 1st terminal cd into Simulation folder and run gazebo with the following command
	$gazebo –verbose cmdquad_test.world
This will open an empty world with a single quadcopter placed at the origin
In the 2nd terminal cd into the build folder in ProjectTest and run the PhysicsEngine executable as follows
	$./PhysicsEngine 1
The argument 1 is switching the physics engine to run a quadcopter model
In the 3rd terminal cd into ardupilot directory and run the following command
	$sim_vehicle.py --v Arducopter --f JSON --map –console

4.2.	 Running the quadcopter model
In the terminal from which you started sim_vehicle.py, once the Autopilot is ready you should be able to control the quadcopter model in gazebo. Type in the following commands
arm uncheck all
arm throttle 
rc 3 1800 (this throttles the quadcopter upward)

rc 1/2/4 1800 control yaw, roll and pitch


# Required Libraries
1. navpy

