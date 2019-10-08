package ropod_ros_msgs;

public interface ropod_demo_plan extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_plan";
  static final java.lang.String _DEFINITION = "std_msgs/Header header\nstring planID\nropod_demo_location[] locations";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  java.lang.String getPlanID();
  void setPlanID(java.lang.String value);
  java.util.List<ropod_ros_msgs.ropod_demo_location> getLocations();
  void setLocations(java.util.List<ropod_ros_msgs.ropod_demo_location> value);
}
