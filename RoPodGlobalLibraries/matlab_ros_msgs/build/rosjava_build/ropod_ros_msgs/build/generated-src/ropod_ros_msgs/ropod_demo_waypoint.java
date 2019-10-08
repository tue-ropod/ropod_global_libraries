package ropod_ros_msgs;

public interface ropod_demo_waypoint extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_waypoint";
  static final java.lang.String _DEFINITION = "string id\nropod_demo_status locationStatus\ngeometry_msgs/Pose waypointPosition";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  java.lang.String getId();
  void setId(java.lang.String value);
  ropod_ros_msgs.ropod_demo_status getLocationStatus();
  void setLocationStatus(ropod_ros_msgs.ropod_demo_status value);
  geometry_msgs.Pose getWaypointPosition();
  void setWaypointPosition(geometry_msgs.Pose value);
}
