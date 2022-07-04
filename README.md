# Human Navigation behavior Simulator (HuNavSim)

A simulator of human navigation behaviors for Robotics based on ROS2.

**Tested in ROS2 Foxy** 

The persons are affected by the obstacles and other persons using the [Social Force Model](https://github.com/robotics-upo/lightsfm).
Besides, some human reactions to the presence of a robot has been included.


## Dependencies

* Yo must download and install the Social Force Model library. Follow the instructions here: lightsfm https://github.com/robotics-upo/lightsfm


## Features

* Realistic human navigation behavior based on an adapted Social Force Model and its extension for groups of people.

* A core of human navigation behaviors which is independent of any simulator. The core can be used in different simulators by means of a wrapper.

* A wrapper for Gazebo Simulator (tested in Gazebo 11) is provided here: XXXXXXX

* The simulator core is integrated in the new ROS2 framework (tested in Foxy distro).

* Despite following the same human navigation model (SFM), the regular navigation behavior for each human agent is different by means of random initialization, or user specification, of some parameters of the model (within reasonable fixed boundaries). That provides a richer human navigation behavior closer to the real world.

* A set of realistic human navigation reactions to the presence of a robot is provided:

    * *regular*: the human treat the robot like another human.
    * *impassive*: the human deal with the robot like a static obstacle.
    * *surprised*: when the human sees the robot, he/she stops walking and starts to look at the robot.
    * *curious*: the human abandons the current navigation goal for a while and starts to approach the robot slowly.
    * *scared*: the human tries to stay far from the robot.
    * *threatening*: the human tries to block the path of the robot by walking in front of it.

* The navigation behavior defined by the user for each human agent is led by a configurable behavior tree.

* Finally, a set of metrics for social navigation evaluation are provided (ongoing work). This set includes the metrics found in the literature plus some other ones. Moreover, the metrics computed can be easily configured and extended by the user.

 

## Configuration

(yaml)


## Example run







```
The parameters that can be configured for each pedestrian are:

### General params

*  ```<velocity>```. Maximum velocity (*m/s*) of the pedestrian.
*  ```<radius>```. Approximate radius of the pedestrian's body (m).
*  ```<animation_factor>```. Factor employed to coordinate the animation with the walking velocity.
* ```<people_distance>```.  Maximum detection distance of the surrounding pedestrians.

### SFM Weights

*  The weight factors that modify the navigation behavior. See the [Social Force Model](https://github.com/robotics-upo/lightsfm) for further information.

### Obstacle params

* ```<ignore_obstacles>```.  All the models that must be ignored as obstacles, must be indicated here. The other actors in the world are included automatically.

### Trajectory params

* ```<trajectory>```. The list of waypoints that the actor must reach must be indicated here. 

	- ```<waypoint>```. Each waypoint must be indicated by its coordinates X, Y, Z in the world frame.
	- ```<cyclic>```. If true, the actor will start the waypoint sequence when the last waypoint is reached.

## Dependencies

* Yo must download and install the Social Force Model library, lightsfm https://github.com/robotics-upo/lightsfm

## Compilation

* This is a ROS2 package so it must be placed inside a ROS2 workspace and compiled through the regular colcon compiler. 
```sh
colcon build --packages-select gazebo_sfm_plugin
```

## Example

An example Gazebo world can be launched through:
```sh
ros2 launch gazebo_sfm_plugin cafe_ros2.launch.py
```
