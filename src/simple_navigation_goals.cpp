#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "std_srvs/Trigger.h"


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
    ros::init(argc, argv, "simple_navigation_goals");

    //tell the action client that we want to spin a thread by default
    MoveBaseClient ac("move_base", true);
  
    //wait for the action server to come up
    while(!ac.waitForServer(ros::Duration(5.0))){
      ROS_INFO("Waiting for the move_base action server to come up");
    }
  
    move_base_msgs::MoveBaseGoal goal;
  
    // //we'll send a goal to the robot to move 1 meter forward
    // goal.target_pose.header.frame_id = "map";
    // goal.target_pose.header.stamp = ros::Time::now();
  
    // goal.target_pose.pose.position.x = 3.0;
    // goal.target_pose.pose.orientation.w = 1.0;
  
    // ROS_INFO("Sending goal");
    // ac.sendGoal(goal);
  
    // ac.waitForResult();
  
    // if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    //   ROS_INFO("Hooray, the base moved 1 meter forward");
    // else
    //   ROS_INFO("The base failed to move forward 1 meter for some reason");
  
    // return 0;

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<std_srvs::Trigger>("goal_coords");
    std_srvs::Trigger::Request srv;
    std_srvs::Trigger::Response srvResp;

    bool first = true;
    ros::Rate rate(0.333); // Every 5 seconds = 0.2

    while (ros::ok()) {

      ROS_INFO("Starting loop");

      if (!first) {

        // ROS_INFO("Hit if");

        bool success = client.call(srv, srvResp);
        // ROS_INFO("Response message: %s", srvResp.message.c_str());

        std::string responseString = srvResp.message.c_str();
        std::string xStr = responseString.substr(0, responseString.find("|"));;
        std::string yStr = responseString.substr(responseString.find("|")+1, responseString.length());

        double goal_x = std::stof(xStr);
        double goal_y = std::stof(yStr);

        ROS_INFO("Goal x: %f", goal_x);
        ROS_INFO("Goal y: %f", goal_y);
      
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();
      
        goal.target_pose.pose.position.x = goal_x;
        goal.target_pose.pose.position.y = goal_y;
        goal.target_pose.pose.orientation.w = 1.0;
      
        ROS_INFO("Sending goal");
        ac.sendGoal(goal);

      } else {

        // ROS_INFO("Hit else");

        first = false;
      
      }
      
      ros::spinOnce();
      rate.sleep();
    }


  }