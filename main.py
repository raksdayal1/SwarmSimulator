import sys, os
import time
import json

from GazeboGen import *

with open('init.json') as json_file:
    data = json.load(json_file)

ip_addr = data['IP_addr']
quadData = data['Vehicles']['Quadcopters']
fwData = data['Vehicles']['Fixedwings']
roverData = data['Vehicles']['Rovers']

total_no_quadrotors = quadData['Instances']
total_no_fixedwings = fwData['Instances']
total_no_rovers = roverData['Instances']

total_no_of_vehicles = total_no_quadrotors + total_no_fixedwings + total_no_rovers

# Extract quaddata
quad_instance_nos = quadData['InstanceNo']
quad_X_Pos = quadData['Xstart']
quad_Y_Pos = quadData['Ystart']
quad_Z_Pos = quadData['Zstart']

# Extract fw data
fw_instance_nos = fwData['InstanceNo']
fw_X_Pos = fwData['Xstart']
fw_Y_Pos = fwData['Ystart']
fw_Z_Pos = fwData['Zstart']

# Check if any instances are repeated

Sdf = CreateRunwayWorld()
Counter = 0
for i in quad_instance_nos:
    port_in = 9002 + 10 * i
    port_out = 9003 + 10 * i
    print(ip_addr, port_in, port_out, quad_X_Pos[Counter], quad_Y_Pos[Counter], quad_Z_Pos[Counter], i)
    Sdf += CreateQuadModel('127.0.0.1', port_in, port_out, quad_X_Pos[Counter], quad_Y_Pos[Counter], quad_Z_Pos[Counter], i)
    Counter += 1

Counter = 0
for j in fw_instance_nos:
    port_in = 9002 + 10 * j
    port_out = 9003 + 10 * j
    print(ip_addr, port_in, port_out, fw_X_Pos[Counter], fw_Y_Pos[Counter], fw_Z_Pos[Counter], j)
    Sdf += CreateFWModel('127.0.0.1', port_in, port_out, fw_X_Pos[Counter], fw_Y_Pos[Counter], fw_Z_Pos[Counter], j)
    Counter += 1

Sdf += GetFooter()

filePath = 'worlds/Created_Universe.world'
if os.path.exists(filePath):
    os.remove(filePath)
    print("Deleted file")
else:
    print("Can not delete the file as it doesn't exists")

with open("worlds/Created_Universe.world", "w") as file:
    file.write(Sdf)

print("Created a new simulation world")

time.sleep(1)

os.system("gazebo --verbose worlds/Created_Universe.world")

if os.path.exists(filePath):
    os.remove(filePath)
    print("Deleted simulation file")


#os.system("./build/PhysicsEngine init.json")

# This works only for iris quadcopters
for vehicle_no in range(0, total_no_of_vehicles):
    ardupilot_cmd = "sim_vehicle.py"
    fixed_params = f"-v ArduCopter -f gazebo-iris -I {vehicle_no} --auto-sysid"
    home_param = "-Z "
