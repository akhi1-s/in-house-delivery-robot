

#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

#include "slam_gmapping.h"

class SlamGMappingNodelet : public nodelet::Nodelet
{
  public:
    SlamGMappingNodelet()  {}

    ~SlamGMappingNodelet() {}
  
    virtual void onInit()
    {
      NODELET_INFO_STREAM("Initialising Slam GMapping nodelet...");
      sg_.reset(new SlamGMapping(getNodeHandle(), getPrivateNodeHandle()));
      NODELET_INFO_STREAM("Starting live SLAM...");
      sg_->startLiveSlam();
    }

  private:  
    boost::shared_ptr<SlamGMapping> sg_;
};

PLUGINLIB_EXPORT_CLASS(SlamGMappingNodelet, nodelet::Nodelet)
