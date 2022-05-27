# SwarmSimulator

1. Requirements
Ubuntu 18 or 20 running natively or in WSL. WSL2 is required to run full 3D graphics
Gazebo 11
ardupilot
ROS Noetic (Optional, on packages used)
Eigen3
navpy


2. How to Install
Make a new build folder using the mkdir command and cd into it
	$mkdir build && cd build
Run cmake
	$cmake ..
Once its completed run the make command
    $make
Once the compilation is successful, you should see an executable called PhysicsEngine and a static library called CMDPlugin.so. Copy the CMDPlugin.so to plugins folder. If any of the ROS based
controllers are compiled, the executable is build in this folder.

3.	Running the code
3.1. Setting the simulation configuration
open the init.json file and set the number of instances, instance numbers and start locations for each of the vehicles. The instances must be uniques and ensure not instance numbers are
repeated or this much cause issues when loading the simulation. Set the Home location value in LLA. The start locations values are in NED coordinates and all units are in meters or degrees.

3.2. Launching the simulation
In the main directory run the launch file using the command
	$python3 sitl_launch.py

This file will launch gazebo viz and multiple instances of ardupilot's sim_vehicle.py and connect with the CMD FDM model. The gazebo visualization window will display the vehicles set in your init.json file
at locations set in the file around the origin. You can connect to Mission Planner or QGroundcontrol to connect to the simulated vehicles and read the telemetry data.

Note: Since Mission planner is not available on Ubuntu, we will launch it on a Windows machine and network it with the system running Ubuntu. To connect to the vehicles, we will connect on TCP to the IP address of
the Ubuntu machine and port 3456.

3.3 Running a controller
Currently there are two controllers that are used in testing: Rigid Formations using Virtual Structures, and Vicsek Formations. There controllers are not designed for Quadrotors and GroundRovers and NOT fixed wing
vehicles. To run the RFVS controller, run the takeoff command for each of the vehicles using MissionPlanner. Then cd in scripts and run
	$python3 VSControl.py

Similarly To run the VicsekSwarm run
	$python3 Vicsek.py
