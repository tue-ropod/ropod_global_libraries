package ropod_ros_msgs;

public interface ropod_demo_sem_location_list extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_sem_location_list";
  static final java.lang.String _DEFINITION = "std_msgs/Header header\nropod_sem_waypoint[] sem_waypoint";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  java.util.List<ropod_ros_msgs.ropod_sem_waypoint> getSemWaypoint();
  void setSemWaypoint(java.util.List<ropod_ros_msgs.ropod_sem_waypoint> value);
}
