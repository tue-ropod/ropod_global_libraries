package ropod_ros_msgs;

public interface sem_waypoint_cmd extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/sem_waypoint_cmd";
  static final java.lang.String _DEFINITION = "std_msgs/Header header\nstring sem_waypoint\nropod_control_primitive[] primitive\n";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  java.lang.String getSemWaypoint();
  void setSemWaypoint(java.lang.String value);
  java.util.List<ropod_ros_msgs.ropod_control_primitive> getPrimitive();
  void setPrimitive(java.util.List<ropod_ros_msgs.ropod_control_primitive> value);
}
