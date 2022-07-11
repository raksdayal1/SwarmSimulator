import sys
import os
import time
import json
import navpy

from subprocess import Popen, DEVNULL

from GazeboGen import *

'''
TODO: Run checks to make sure none of the instances are repeated
'''

with open('../init.json') as json_file:
    data = json.load(json_file)

ip_addr = data['IP_addr']
quadData = data['Vehicles']['Quadcopters']
fwData = data['Vehicles']['Fixedwings']
roverData = data['Vehicles']['Rovers']

total_no_quadrotors = quadData['Instances']
total_no_fixedwings = fwData['Instances']
total_no_rovers = roverData['Instances']

total_no_of_vehicles = total_no_quadrotors + total_no_fixedwings + total_no_rovers

# Extract quad data
quad_instances = quadData['InstanceNo']
quad_X_Pos = quadData['Xstart']
quad_Y_Pos = quadData['Ystart']
quad_Z_Pos = quadData['Zstart']

# Extract fw data
fw_instances = fwData['InstanceNo']
fw_X_Pos = fwData['Xstart']
fw_Y_Pos = fwData['Ystart']
fw_Z_Pos = fwData['Zstart']

# Extract rover data (model spawning still needed)
rover_instances = roverData['InstanceNo']
rover_X_Pos = roverData['Xstart']
rover_Y_Pos = roverData['Ystart']
rover_Z_Pos = roverData['Zstart']

# Get the home location
lat_ref = data["Home"]["latitude"]
lon_ref = data["Home"]["longitude"]
alt_ref = data["Home"]["altitude"]

# Get totals for testing
all_instances = quad_instances + fw_instances + rover_instances
all_Pos_values = quad_X_Pos + quad_Y_Pos + quad_Z_Pos + fw_X_Pos + fw_Y_Pos + fw_Z_Pos + rover_X_Pos + rover_Y_Pos + \
                 rover_Z_Pos

# Series of tests to ensure init.json input matches up
if total_no_of_vehicles != len(all_instances):
    sys.exit('\x1b[0;31;40m' + 'Error: Each Instance must have exactly one corresponding InstanceNo' + '\x1b[0m')
elif len(all_instances) != len(set(all_instances)):
    sys.exit('\x1b[0;31;40m' + 'Error: All vehicle Instance Numbers must be unique' + '\x1b[0m')
elif len(all_instances) * 3 != len(all_Pos_values):
    sys.exit('\x1b[0;31;40m' + 'Error: Each instance must have exactly one corresponding Xstart, Ystart, and Zstart' +
             '\x1b[0m')

Sdf = CreateRunwayWorld()
Counter = 0

Ap_Commands = []
mavproxy_exe = ["mavproxy.py", "--out=tcpin:0.0.0.0:3456"]

for i in quad_instances:
    port_in = 9002 + 10 * i
    port_out = 9003 + 10 * i
    Sdf += CreateQuadModel('127.0.0.1', port_in, port_out, quad_X_Pos[Counter], quad_Y_Pos[Counter],
                           quad_Z_Pos[Counter], i)
    ned = [quad_X_Pos[Counter], quad_Y_Pos[Counter], quad_Z_Pos[Counter]]
    lla_out = navpy.ned2lla(ned, lat_ref, lon_ref, alt_ref, latlon_unit='deg', alt_unit='m', model='wgs84')

    Ap_Commands.append(["sim_vehicle.py", "-vArduCopter", "-fgazebo-iris", f"-I{i}",
              "--auto-sysid", f"--custom-location={lla_out[0]},{lla_out[1]},{lla_out[2]},0"])

    udp_port = 14550 + 10 * i
    mavproxy_exe.append(f"--master=udp:127.0.0.1:{udp_port}")
    Counter += 1

Counter = 0
for j in fw_instances:
    port_in = 9002 + 10 * j
    port_out = 9003 + 10 * j
    Sdf += CreateFWModel('127.0.0.1', port_in, port_out, fw_X_Pos[Counter], fw_Y_Pos[Counter], fw_Z_Pos[Counter], j)
    ned = [fw_X_Pos[Counter], fw_Y_Pos[Counter], fw_Z_Pos[Counter]]
    lla_out = navpy.ned2lla(ned, lat_ref, lon_ref, alt_ref, latlon_unit='deg', alt_unit='m', model='wgs84')

    Ap_Commands.append(["sim_vehicle.py", "-v ArduPlane", "-f gazebo-zephyr", f"-I{j}",
              "--auto-sysid ", f"--custom-location=({lla_out[0]},{lla_out[1]},{lla_out[2]},0)"])

    udp_port = 14550 + 10 * j
    mavproxy_exe.append(f"--master=udp:127.0.0.1:{udp_port}")
    Counter += 1

Sdf += GetFooter()

filePath = '../worlds/Created_Universe.world'
if os.path.exists(filePath):
    os.remove(filePath)
    print("Deleted file")
else:
    print("Can not delete the file as it doesn't exists")

with open("../worlds/Created_Universe.world", "w") as file:
    file.write(Sdf)

print("Created a new simulation world")

time.sleep(1)
Ap_Commands.insert(0, ["gazebo", "--verbose", "../worlds/Created_Universe.world"])
Ap_Commands.append(mavproxy_exe)

filePathapm = '../simulator_ws/src/cowboy_swarm/launch/apm2_multitest.launch' # need to correct path
if os.path.exists(filePathapm):
    os.remove(filePathapm)
    print("Deleted roslaunch APM file")
else:
    print("Can not delete the roslaunch APM file as it doesn't exists")

APMRos = "<launch>\n"
for id in quad_instances + fw_instances:
    APMRos += f"""<node pkg="mavros" type="mavros_node" name="mavros" required="false" clear_params="true" output="screen" respawn="true" ns="/drone{id+1}">
		<param name="fcu_url" value="udp://127.0.0.1:{14551+10*id}@" />
		<param name="gcs_url" value="" />
		<param name="target_system_id" value="{id+1}" />
		<param name="target_component_id" value="1" />
		<param name="fcu_protocol" value="v2.0" />

		<!-- load blacklist, config -->
		<rosparam command="load" file="$(find mavros)/launch/apm_pluginlists.yaml" />
		<rosparam command="load" file="$(find mavros)/launch/apm_config.yaml" />
	</node>"""
APMRos += "\n</launch>"

with open(filePathapm, "w") as file:
    file.write(APMRos)

print("Created a new ROSAPM file")


#Ap_Commands.append(["roslaunch", "cowboy_swarm", "apm2_multitest.launch"])
# Ap_Commands.append(["python3", "simulation_setup.py", f"{lat_ref}", f"{lon_ref}", f"{alt_ref}"])

print(Ap_Commands)

procs = []
for cmd in Ap_Commands:
    procs.append(Popen(cmd, stdout=DEVNULL, stderr=DEVNULL))
    time.sleep(1)

procs.append(Popen(["roslaunch", "cowboy_swarm", "apm2_multitest.launch"]))

procs[0].wait()  # Only waiting on gazebo window

if os.path.exists(filePath):
    os.remove(filePath)
    print("Deleted simulation file")

time.sleep(1)
if os.path.exists(filePathapm):
    os.remove(filePathapm)
    print("Deleted Roslaunch apm simulation file")

exit_cmds = ['fuser -k 11311/tcp', 'fuser -k 3456/tcp', 'pkill mavproxy.py', 'pkill mavproxy.py']
for c in exit_cmds:
    kill = Popen(f"{c}", shell=True)
    kill.wait()
    print(kill.returncode)

sys.exit('\x1b[1;33m' + "Now Exiting Simulation" + '\x1b[0m')
