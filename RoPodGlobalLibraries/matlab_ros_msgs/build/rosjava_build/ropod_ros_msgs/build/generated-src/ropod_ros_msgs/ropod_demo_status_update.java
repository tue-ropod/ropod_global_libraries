package ropod_ros_msgs;

public interface ropod_demo_status_update extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_status_update";
  static final java.lang.String _DEFINITION = "string id\nropod_demo_status status\n";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  java.lang.String getId();
  void setId(java.lang.String value);
  ropod_ros_msgs.ropod_demo_status getStatus();
  void setStatus(ropod_ros_msgs.ropod_demo_status value);
}
