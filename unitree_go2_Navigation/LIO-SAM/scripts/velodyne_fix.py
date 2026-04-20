import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
import sensor_msgs_py.point_cloud2 as pc2

class TransposeCloud(Node):
    def __init__(self):
        super().__init__('transpose_cloud')
        self.sub = self.create_subscription(
            PointCloud2, '/velodyne_points', self.callback, 10)
        self.pub = self.create_publisher(
            PointCloud2, '/velodyne_points_fixed', 10)

    def callback(self, msg):
        width = msg.width
        height = msg.height

        points = list(pc2.read_points(msg, field_names=None, skip_nans=False))

        # reshape
        grid = []
        idx = 0
        for h in range(height):
            row = []
            for w in range(width):
                row.append(points[idx])
                idx += 1
            grid.append(row)

        # transpose
        transposed = list(zip(*grid))

        new_points = []
        for row in transposed:
            new_points.extend(row)

        new_msg = pc2.create_cloud(msg.header, msg.fields, new_points)
        new_msg.height = width
        new_msg.width = height

        self.pub.publish(new_msg)

# ★ これがないと即終了する
def main():
    rclpy.init()
    node = TransposeCloud()
    rclpy.spin(node)   # ← ここ重要
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()