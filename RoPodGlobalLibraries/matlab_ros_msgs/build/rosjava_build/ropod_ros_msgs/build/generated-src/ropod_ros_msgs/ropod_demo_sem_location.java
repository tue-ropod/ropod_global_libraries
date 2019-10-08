package ropod_ros_msgs;

public interface ropod_demo_sem_location extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_sem_location";
  static final java.lang.String _DEFINITION = "string command\nstring location";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  java.lang.String getCommand();
  void setCommand(java.lang.String value);
  java.lang.String getLocation();
  void setLocation(java.lang.String value);
}
