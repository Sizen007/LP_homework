import rclpy
from rclpy.node import Node 
from std_msgs.msg import UInt32
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor

class CustomizeNode(Node):
    def __init__(self,name):
        super().__init__(name)
        #sizen
        callback_group_1 = MutuallyExclusiveCallbackGroup()
        callback_group_2 = MutuallyExclusiveCallbackGroup()
        
        self.total = 0
        #sizen
        self.sub_1 = self.create_subscription(UInt32, "num", self.recv_callback,10, callback_group=callback_group_1) 
        self.pub_1 = self.create_publisher(UInt32,"result", 10) 

        self.count = 0
        self.timer_period = 2
        #sizen
        self.timer = self.create_timer(self.timer_period, self.timer_callback, callback_group=callback_group_2)

    def recv_callback(self,digital):
        number = digital.data
        if number <= 100:
            self.total += number
            self.get_logger().info('累加结果: %d' % self.total)

    def timer_callback(self):
        msg = UInt32()
        msg.data = self.total
        self.pub_1.publish(msg) 
        self.get_logger().info('发布结果: %d' % msg.data) 

#sizen  
def main(args=None):
    rclpy.init(args=args)
    node = CustomizeNode("subscribe_01")
    executor = MultiThreadedExecutor()
    executor.add_node(node)
    executor.spin()

if __name__ == '__main__':
    main()