package ropod_ros_msgs;

public interface ropod_demo_status extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "ropod_ros_msgs/ropod_demo_status";
  static final java.lang.String _DEFINITION = "string status\nuint16 sequenceNumber\nuint16 totalNumber\n";
  static final boolean _IS_SERVICE = false;
  static final boolean _IS_ACTION = false;
  java.lang.String getStatus();
  void setStatus(java.lang.String value);
  short getSequenceNumber();
  void setSequenceNumber(short value);
  short getTotalNumber();
  void setTotalNumber(short value);
}
