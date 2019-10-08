package ropod_ros_msgs;

public interface ropod_control_primitive extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_control_primitive";
  static final java.lang.String _DEFINITION = "std_msgs/Header header\nstring behaviour\ngeometry_msgs/PoseStamped[] poses\n";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  java.lang.String getBehaviour();
  void setBehaviour(java.lang.String value);
  java.util.List<geometry_msgs.PoseStamped> getPoses();
  void setPoses(java.util.List<geometry_msgs.PoseStamped> value);
}
