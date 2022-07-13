
from pymavlink import mavutil
import time
import threading
from math import sqrt

# Enter Instance Numbers from init.json
InstanceNo = [0,1,2,3]

connections = {}


# Creates a connection for each drone, adds it to the dictionary "connections". Confirms connection with heartbeat
def create_connections(instance):
    connections[f'drone{instance}'] = mavutil.mavlink_connection(f'udpin:localhost:{14551 + 10 * instance}')
    initialize = connections[f'drone{instance}']
    initialize.wait_heartbeat()
    print(f'[HEARTBEAT] Heartbeat confirmed from Instance Number {instance}')



# Sends arm command, sets mode to guided, and takes off to input altitude. NOTE: Drone must register GPS location before it can takeoff
def arm_and_takeoff(instance, altitude):
    connection = connections[f'drone{instance}']
    position = get_position(instance)

    while abs(abs(position.z) - abs(altitude)) >= 0.25:
        connection.mav.command_long_send(connection.target_system, connection.target_component, mavutil.mavlink.MAV_CMD_DO_SET_MODE, 0, 1, 4, 0, 0, 0, 0, 0)
        msg = connection.recv_match(type='COMMAND_ACK', blocking=True)
        time.sleep(1)

        if msg.result == 0:
            connection.mav.command_long_send(connection.target_system, connection.target_component, mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0)
            msg = connection.recv_match(type='COMMAND_ACK', blocking=True)
            time.sleep(1)
            
            if msg.result == 0:
                connection.mav.command_long_send(connection.target_system, connection.target_component, mavutil.mavlink.MAV_CMD_NAV_TAKEOFF, 0, 0, 0, 0, 0, 0, 0, altitude)
                msg = connection.recv_match(type='COMMAND_ACK', blocking=True)

                if msg.result == 0:
                    print('\x1b[1;33m' + f'[TAKEOFF ACCEPTED] drone{instance} takeoff command accepted' + '\x1b[0m')

        position = get_position(instance)
        time.sleep(2)

    print('\x1b[32m' + f'[TAKEOFF COMPLETE] drone{instance} takeoff complete' + '\x1b[0m')



def get_position(instance):
    connection = connections[f'drone{instance}']
    position = connection.recv_match(type='LOCAL_POSITION_NED', blocking=True)
    return position



def travel_to_waypoints(instance, x_list, y_list, z_list):
    connection = connections[f'drone{instance}']
    counter = 0

    for i in x_list:
        print('\x1b[1;33m' + f'[WAYPOINT ACCEPTED] drone{instance} to ({x_list[counter]}, {y_list[counter]}, {z_list[counter]})' + '\x1b[0m')

        position = get_position(instance)

        previous_error_x = 0
        previous_error_y = 0
        previous_error_z = 0

        sum_error_x = 0
        sum_error_y = 0
        sum_error_z = 0



        while sqrt((position.x - x_list[counter])**2 + (position.y - y_list[counter])**2 + (position.z - z_list[counter])**2) >= 1:
            position = get_position(instance)

            current_error_x = x_list[counter] - position.x
            current_error_y = y_list[counter] - position.y
            current_error_z = z_list[counter] - position.z

            dx = current_error_x - previous_error_x
            dy = current_error_y - previous_error_y
            dz = current_error_z - previous_error_z

            sum_error_x = sum_error_x + current_error_x * 0.1
            sum_error_y = sum_error_y + current_error_y * 0.1
            sum_error_z = sum_error_z + current_error_z * 0.1

            if sum_error_x >= 1:
                sum_error_x = 1
            elif sum_error_x <= -1:
                sum_error_x = -1
            
            if sum_error_y >= 1:
                sum_error_y = 1
            elif sum_error_y <= -1:
                sum_error_y = -1

            if sum_error_z >= 1:
                sum_error_z = 1
            elif sum_error_z <= -1:
                sum_error_z = -1
            
            #print(f'[X-Direction] P: {current_error_x}, I: {sum_error_x}, D: {sum_error_z}')

            # Final eq
            velocity_x = (0.8 * current_error_x) + (5.6 * dx) + (0.5 * sum_error_x)
            velocity_y = (0.8 * current_error_y) + (5.6 * dy) + (0.5 * sum_error_y)
            velocity_z = (0.8 * current_error_z) + (5.6 * dz) + (0.5 * sum_error_z)

            if velocity_x >= 8:
                velocity_x = 8
            elif velocity_x <= -8:
                velocity_x = -8
            
            if velocity_y >= 8:
                velocity_y = 8
            elif velocity_y <= -8:
                velocity_y = -8
            
            if velocity_z >= 8:
                velocity_z = 8
            elif velocity_z <= -8:
                velocity_z = -8


            if abs(current_error_x) <= 0.3:
                velocity_x = 0

            if abs(current_error_y) <= 0.3:
                velocity_y = 0
            
            if abs(current_error_z) <= 0.3:
                velocity_z = 0
            
            
            #print(f"Here is what we're writing: X: {velocity_x}, Y: {velocity_y}, Z: {velocity_z}")

            connection.mav.send(mavutil.mavlink.MAVLink_set_position_target_local_ned_message(10, connection.target_system, connection.target_component, mavutil.mavlink.MAV_FRAME_LOCAL_NED, int(0b110111000111), 0, 0, 0, (velocity_x), (velocity_y), -(velocity_z), 0, 0, 0, 0, 0))

            previous_error_x = current_error_x
            previous_error_y = current_error_y
            previous_error_z = current_error_z

            #time.sleep(0.25)
        
        print('\x1b[1;32m' + f'[WAYPOINT COMLPETE] drone{instance} at ({x_list[counter]}, {y_list[counter]}, {z_list[counter]})' + '\x1b[0m')
        counter += 1
        time.sleep(0.2)



def broadcast_coordinates(instance):
    pass

    # WIP sending position info between drones
    position = get_position(instance)
    
    all_instances = InstanceNo
    all_instances.remove(instance)
    
    for i in InstanceNo:
        connection = connections[f'drone{i}']
        # Send position to a port on each drone
    
    # Broadcast every second
    time.sleep(1)
    


# Creates all connections in the dictionary "connections"
for i in InstanceNo:
    create_connections(i)


# Calls the arm_and_takeoff function for each drone, sending them to desired_altitude
waitlist = []
desired_altitude = 10
for i in InstanceNo:
    t = threading.Thread(target=arm_and_takeoff,args=[i, desired_altitude])
    t.start()
    waitlist.append(t)

# Waits for each thread to finish
for thread in waitlist:
    thread.join()
    waitlist.remove(thread)

time.sleep(3)



# Creates a list of xyz coordinates, calls travel_to_waypoints for each instance
x_list = [20, 20, -20, -20]
y_list = [20, -20, -20, 20]
z_list = [-10, -10, -10, -10]

for i in InstanceNo:
    t = threading.Thread(target=travel_to_waypoints, args=[i,x_list,y_list,z_list])
    t.start()
    waitlist.append(t)

for thread in waitlist:
    thread.join()
    waitlist.remove(thread)



# Sends a set of coordinates/calls travel_to_waypoints for drone 2 specifically
x_list = [30, 40, -50, -60]
y_list = [30, -40, -50, 60]
z_list = [-10, -10, -10, -10]

t = threading.Thread(target=travel_to_waypoints, args=[2,x_list,y_list,z_list])
t.start()
t.join()


print('\x1b[1;32m' + '-- Mission Complete --' + '\x1b[0m')


