import os
from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
import yaml

os.environ['RCUTILS_CONSOLE_OUTPUT_FORMAT'] = '{time}: [{name}] [{severity}]\t{message}'
# Verbose log:
#os.environ['RCUTILS_CONSOLE_OUTPUT_FORMAT'] = '{time}: [{name}] [{severity}]\t{message} ({function_name}() at {file_name}:{line_number})'

def generate_launch_description():
    
    # Read original yaml file and pass parameters directly
    with open('/home/colcon_ws/src/septentrio_gnss_driver/config/rover.yaml', 'r') as f:
        config_data = yaml.safe_load(f)

    composable_node = ComposableNode(
        name='septentrio_gnss_driver',
        package='septentrio_gnss_driver', 
        plugin='rosaic_node::ROSaicNode',
        parameters=[config_data])

    container = ComposableNodeContainer(
        name='septentrio_gnss_driver_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container_isolated',
        emulate_tty=True,
        composable_node_descriptions=[composable_node],
        output='screen'
    )

    return LaunchDescription([container])