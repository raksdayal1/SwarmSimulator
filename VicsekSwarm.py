import rospy
from mavros_msgs.msg import CommandLong, PositionTarget
from sensor_msgs.msg import NavSatFix
from geometry_msgs.msg import PoseWithCovarianceStamped

class Vehicle:
    def __init__(self, sys_id):
        rospy.Subscriber(f"/iris{sys_id}/global_position/local", PoseWithCovarianceStamped, self.StateCb)
        self.pubcontrol = rospy.Publisher(f"/iris{sys_id}/setpoint_raw/local", PositionTarget, queue_size=10)

        self.XPos = None
        self.YPos = None
        self.ZPos = None
        self.Heading = None

    def StateCb(self, data):

        self.XPos = data.pose.pose.position.x
        self.YPos = data.pose.pose.position.y
        self.ZPos = data.pose.pose.position.z

        #self.Heading = data.pose.pose.orientation

        rospy.loginfo(f"Xpos = {data.pose.pose.position.x}, Ypos = {data.pose.pose.position.y},"
                      f"Zpos = {data.pose.pose.position.z} ")

    def WriteControl(self, control, rate):
        self.pubcontrol.publish(control)
        rate.sleep()


class Controller:
    def __init__(self):
        rospy.init_node('ControllerNode', anonymous=True)
        self.rate = rospy.Rate(10)
        self.vehicle_list = []

        for i in range(0,5):
            v = Vehicle(i)
            self.vehicle_list.append(v)

    def Run(self):
        while not rospy.is_shutdown():
            '''
                Write controller here
            '''
            cmd = None

            self.vehicle_list[0].WriteControl(cmd, self.rate)

