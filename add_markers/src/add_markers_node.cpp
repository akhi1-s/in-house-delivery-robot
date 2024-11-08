
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"
#include <cmath>

double pickUpPos[2]  = {7.5, -10.0};
double dropOffPos[2] = {17.3, -3.7};

double pose[2] = {0, 0};  // current pose

void get_current_pose(const nav_msgs::Odometry::ConstPtr& msg)
{
  pose[0] = msg->pose.pose.position.x;
  pose[1] = msg->pose.pose.position.y;
}

double distToCurrentPos(double goalPos[2])
{
  double dx = goalPos[0] - pose[0];
  double dy = goalPos[1] - pose[1];
  return sqrt(dx*dx + dy*dy);
}

bool reach_pick_up()
{
  return distToCurrentPos(pickUpPos) < 0.8;
}

bool reach_drop_zone()
{
  return distToCurrentPos(dropOffPos) < 0.2;
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  ros::Subscriber pose_sub = n.subscribe("odom", 10, get_current_pose);

  uint32_t shape = visualization_msgs::Marker::CUBE;

  enum State {
    PICKUP,  // going to pick up zon
    CARRY,   // carry to drop zone
    DROP,    // already drop
  } state = PICKUP;

  ROS_INFO("Going to pick up zone ... ");
  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "odom";
    marker.header.stamp = ros::Time::now();

    marker.ns = "basic_shapes";
    marker.id = 0;

    marker.type = shape;

    marker.action = visualization_msgs::Marker::ADD;

    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    /*while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }*/

    ros::spinOnce();

    if (state == PICKUP) {
      marker.action = visualization_msgs::Marker::ADD;
      marker.pose.position.x = pickUpPos[0];
      marker.pose.position.y = pickUpPos[1];
      marker_pub.publish(marker);
      if (reach_pick_up()) {
        sleep(5);
        ROS_INFO("Carrying to drop zone ... ");
        state = CARRY;
      }
    }
    else if (state == CARRY) {
      marker.action = visualization_msgs::Marker::DELETE;
      marker.pose.position.x = dropOffPos[0];
      marker.pose.position.y = dropOffPos[1];
      marker_pub.publish(marker);
      if (reach_drop_zone()) {
        ROS_INFO("Reached drop zone. ");
        state = DROP;
      }
    }
    else /* state == DROP */ {
      marker.action = visualization_msgs::Marker::ADD;
      marker.pose.position.x = dropOffPos[0];
      marker.pose.position.y = dropOffPos[1];
      marker_pub.publish(marker);
    }
  }
}
