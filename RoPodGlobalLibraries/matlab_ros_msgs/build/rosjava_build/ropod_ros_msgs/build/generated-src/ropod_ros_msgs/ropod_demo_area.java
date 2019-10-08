package ropod_ros_msgs;

public interface ropod_demo_area extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_area";
  static final java.lang.String _DEFINITION = "string areaName\nstring id\nropod_demo_status locationStatus\nropod_demo_waypoint[] waypoints";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  java.lang.String getAreaName();
  void setAreaName(java.lang.String value);
  java.lang.String getId();
  void setId(java.lang.String value);
  ropod_ros_msgs.ropod_demo_status getLocationStatus();
  void setLocationStatus(ropod_ros_msgs.ropod_demo_status value);
  java.util.List<ropod_ros_msgs.ropod_demo_waypoint> getWaypoints();
  void setWaypoints(java.util.List<ropod_ros_msgs.ropod_demo_waypoint> value);
}
