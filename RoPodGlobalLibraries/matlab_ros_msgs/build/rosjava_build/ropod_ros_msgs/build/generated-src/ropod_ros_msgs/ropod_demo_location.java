package ropod_ros_msgs;

public interface ropod_demo_location extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_location";
  static final java.lang.String _DEFINITION = "string command\nstring locationName\nstring id\nropod_demo_status locationStatus\nropod_demo_area[] areas";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  java.lang.String getCommand();
  void setCommand(java.lang.String value);
  java.lang.String getLocationName();
  void setLocationName(java.lang.String value);
  java.lang.String getId();
  void setId(java.lang.String value);
  ropod_ros_msgs.ropod_demo_status getLocationStatus();
  void setLocationStatus(ropod_ros_msgs.ropod_demo_status value);
  java.util.List<ropod_ros_msgs.ropod_demo_area> getAreas();
  void setAreas(java.util.List<ropod_ros_msgs.ropod_demo_area> value);
}
