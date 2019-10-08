package ropod_ros_msgs;

public interface ropod_sem_waypoint extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_sem_waypoint";
  static final java.lang.String _DEFINITION = "string command\nstring location\n";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  java.lang.String getCommand();
  void setCommand(java.lang.String value);
  java.lang.String getLocation();
  void setLocation(java.lang.String value);
}
