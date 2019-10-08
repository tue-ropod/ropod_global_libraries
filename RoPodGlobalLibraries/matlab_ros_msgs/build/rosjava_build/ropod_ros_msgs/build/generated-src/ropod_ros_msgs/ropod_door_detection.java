package ropod_ros_msgs;

public interface ropod_door_detection extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_door_detection";
  static final java.lang.String _DEFINITION = "Header header\nstring id\nstring type\nbool open\nbool closed\nbool undetectable";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  java.lang.String getId();
  void setId(java.lang.String value);
  java.lang.String getType();
  void setType(java.lang.String value);
  boolean getOpen();
  void setOpen(boolean value);
  boolean getClosed();
  void setClosed(boolean value);
  boolean getUndetectable();
  void setUndetectable(boolean value);
}
