

#include <ros/ros.h>

#include "slam_gmapping.h"

int
main(int argc, char** argv)
{
  ros::init(argc, argv, "slam_gmapping");

  SlamGMapping gn;
  gn.startLiveSlam();
  ros::spin();

  return(0);
}

