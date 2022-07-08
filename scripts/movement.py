from pymavlink import mavutil
import time
from math import sqrt


# Starts a connection listening to the udp port, waits for heartbeat to confirm
connection = mavutil.mavlink_connection('udpin:localhost:14551')
connection.wait_heartbeat()
print(f'Received heartbeat from system (system {connection.target_system} component {connection.target_component})')


x_list = [20, 20, -20, -20]
y_list = [20, -20, -20, 20]
z_list = [-10, -10, -10, -10, -10]

counter = 0
start = time.time()

for i in x_list:
    print('\x1b[1;33m' + f'GOAL: ({x_list[counter]}, {y_list[counter]}, {z_list[counter]})' + '\x1b[0m')
    position = connection.recv_match(type='LOCAL_POSITION_NED', blocking=True)

    while sqrt((position.x - x_list[counter])**2 + (position.y - y_list[counter])**2 + (position.z - z_list[counter])**2) >= 1:
        connection.mav.send(mavutil.mavlink.MAVLink_set_position_target_local_ned_message(10, connection.target_system, connection.target_component,   mavutil.mavlink.MAV_FRAME_LOCAL_NED, int(0b110111111000), x_list[counter], y_list[counter], z_list[counter], 0, 0, 0, 0, 0, 0, 0, 0))

        position = connection.recv_match(type='LOCAL_POSITION_NED', blocking=True)
        current_coords = position.x + position.y + position.z

    
    print(f'GOAL REACHED, CURRENT POSITION ({position.x}, {position.y}, {position.z})')
    counter += 1

end = time.time()
print('\x1b[1;33m' + f'TOTAL TIME: {end - start}' + '\x1b[0m')
