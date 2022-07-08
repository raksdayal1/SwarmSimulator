from pymavlink import mavutil
import time


# Starts a connection listening to the udp port
connection = mavutil.mavlink_connection('udpin:localhost:14551')


# Waits to receive heartbeat from the aircraft; prints system and component ID of the remote system for the link
connection.wait_heartbeat()
print(f'Heartbeat from system (system {connection.target_system} component {connection.target_component})')

# Sends arm command, receives acknowledgement from the vehicle
connection.mav.command_long_send(connection.target_system, connection.target_component, mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0,0,0,0,0,0)
msg = connection.recv_match(type='COMMAND_ACK', blocking=True)
print(msg)

time.sleep(3)

# Sets mode to guided
connection.mav.command_long_send(connection.target_system, connection.target_component, mavutil.mavlink.MAV_CMD_DO_SET_MODE, 0, 1, 4, 0, 0, 0, 0, 0)
msg = connection.recv_match(type='COMMAND_ACK', blocking=True)
print(msg)

time.sleep(2)

# Sends takeoff command, receives acknowledgement
connection.mav.command_long_send(connection.target_system, connection.target_component, mavutil.mavlink.MAV_CMD_NAV_TAKEOFF, 0, 0, 0, 0, 0, 0, 0, 10)
msg = connection.recv_match(type='COMMAND_ACK', blocking=True)
print(msg)
