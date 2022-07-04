#include "rclcpp/rclcpp.hpp"
#include <ament_index_cpp/get_package_prefix.hpp>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <std_msgs/msg/color_rgba.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <visualization_msgs/msg/marker_array.hpp>

#include "hunav_agent_manager/bt_functions.hpp"

#include "hunav_msgs/msg/agent.hpp"
#include "hunav_msgs/msg/agents.hpp"
//#include "human_nav_msgs/srv/is_robot_visible.hpp"
#include "hunav_msgs/srv/compute_agent.hpp"
#include "hunav_msgs/srv/compute_agents.hpp"
#include "hunav_msgs/srv/move_agent.hpp"

#include "hunav_agent_manager/time_expired_condition.hpp"
//#include "nav2_behavior_tree/plugins/condition/time_expired_condition.hpp"

#include <functional>

// Social Force Model
#include <lightsfm/sfm.hpp>

namespace hunav_agent_manager {

class BTnode : public rclcpp::Node {
public:
  /**
   * @brief Construct a new BTnode object
   *
   */
  BTnode();
  /**
   * @brief Destroy the BTnode object
   *
   */
  ~BTnode();

  // void registerBTNodes(BT::BehaviorTreeFactory &factory);
  void registerBTNodes();

protected:
  void initializeBehaviorTree(hunav_msgs::msg::Agents agents);
  BT::NodeStatus tree_tick(double dt);

  BT::NodeStatus tree_tick(int id);
  BT::NodeStatus tree_tick(int id, double dt);
  /**
   * @brief ROS service to compute the new state of the agents
   *
   * @param request contains the current state of the agents
   * @param response contains the updated state of the agents
   */
  void computeAgentsService(
      const std::shared_ptr<hunav_msgs::srv::ComputeAgents::Request> request,
      std::shared_ptr<hunav_msgs::srv::ComputeAgents::Response> response);

  /**
   * @brief ROS service to compute the new state of one agent
   *
   * @param request contains the id of the agent
   * @param response contains the updated state of the agent
   */
  void computeAgentService(
      const std::shared_ptr<hunav_msgs::srv::ComputeAgent::Request> request,
      std::shared_ptr<hunav_msgs::srv::ComputeAgent::Response> response);

  /**
   * @brief ROS service to compute the new state of one agent
   *
   * @param request contains the current state of the agents and the id of the
   * agent to move
   * @param response contains the updated state of the agent
   */
  void moveAgentService(
      const std::shared_ptr<hunav_msgs::srv::MoveAgent::Request> request,
      std::shared_ptr<hunav_msgs::srv::MoveAgent::Response> response);

  /**
   * @brief Publish an arrow marker in Rviz representing a force
   * @param index id of the marker
   * @param name name of the agent to be used as namespace
   * @param frame string of the tf frame
   * @param t time stamp
   * @param p point of the marker
   * @param color RGB color of the marker
   * @param force force to be represented
   * @param markers markerArray in which the arrow will be added
   * @return none
   */
  void publishForceMarker(unsigned index, std::string name, std::string frame,
                          rclcpp::Time t, geometry_msgs::msg::Point p,
                          const std_msgs::msg::ColorRGBA &color,
                          const utils::Vector2d &force,
                          visualization_msgs::msg::MarkerArray &markers);

  /**
   * @brief fill a ColorRGBA message
   * @param r value of the red componet [0-1]
   * @param g value of the green component [0-1]
   * @param b value of the blue component [0-1]
   * @param a transparency of the color [0-1]
   * @return a ROS ColorRGBA message
   */
  std_msgs::msg::ColorRGBA getColor(double r, double g, double b, double a);

  /**
   * @brief Publish the TF of the robot and agents
   *
   * @param t time stamp
   * @param robot robot state msg
   * @param msg agent states msg
   */
  void publish_agents_tf(rclcpp::Time t,
                         const hunav_msgs::msg::Agent::SharedPtr robot,
                         const hunav_msgs::msg::Agents::SharedPtr msg);

  /**
   * @brief Publish the TF of the robot and agents
   *
   * @param t time stamp
   * @param robot robot state msg
   * @param msg agent states msg
   */
  void publish_agent_states(rclcpp::Time t,
                            const hunav_msgs::msg::Agents::SharedPtr msg);

  /**
   * @brief Publish the set of SFM forces in RViz
   * @param t time stamp
   * @param msg agents msg
   * @return none
   */
  void publish_agents_forces(rclcpp::Time t,
                             const hunav_msgs::msg::Agents::SharedPtr msg);

  sfm::Forces getAgentForces(int id) { return btfunc_.getAgentForces(id); };

  // Services provided
  rclcpp::Service<hunav_msgs::srv::ComputeAgents>::SharedPtr agents_srv_;
  rclcpp::Service<hunav_msgs::srv::ComputeAgent>::SharedPtr agent_srv_;
  rclcpp::Service<hunav_msgs::srv::MoveAgent>::SharedPtr move_agent_srv_;

  // void agentsCallback(const hunav_msgs::msg::Agents &msg);
  // void agentRobotCallback(const hunav_msgs::msg::Agent &msg);

  // rclcpp::Subscription<hunav_msgs::msg::Agents>::SharedPtr agents_sub_;
  // rclcpp::Subscription<hunav_msgs::msg::Agent>::SharedPtr
  // agent_robot_sub_;
  // rclcpp::Publisher<hunav_msgs::msg::Agent>::SharedPtr agent_pub_;
  // std::vector<rclcpp::Publisher<hunav_msgs::msg::Agent>::SharedPtr>
  //    publishers_;

  bool initialized_;
  BTfunctions btfunc_;
  std::string pkg_shared_tree_dir_;
  // std::vector<BT::Tree> trees_;
  std::unordered_map<int, BT::Tree> trees_;

  rclcpp::Time prev_time_;

  // node parameters
  bool pub_tf_;
  bool pub_forces_;
  bool pub_agent_states_;

  // BT::Tree tree_;
  // std::unique_ptr<BT::ParallelNode> root_;
  // std::unique_ptr<BT::SequenceNode> root_;
  BT::BehaviorTreeFactory factory_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr
      forces_publisher_;
  rclcpp::Publisher<hunav_msgs::msg::Agents>::SharedPtr state_publisher_;
};

} // namespace hunav_agent_manager