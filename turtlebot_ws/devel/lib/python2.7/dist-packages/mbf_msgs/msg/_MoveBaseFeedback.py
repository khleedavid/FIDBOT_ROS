# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from mbf_msgs/MoveBaseFeedback.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import geometry_msgs.msg
import std_msgs.msg

class MoveBaseFeedback(genpy.Message):
  _md5sum = "1b30e381361670e9521046df439847e2"
  _type = "mbf_msgs/MoveBaseFeedback"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======

# Outcome of most recent controller cycle. Same values as in MoveBase or ExePath result.
uint32 outcome
string message

float32 dist_to_goal
float32 angle_to_goal
geometry_msgs/PoseStamped current_pose
geometry_msgs/TwistStamped last_cmd_vel  # last command calculated by the controller


================================================================================
MSG: geometry_msgs/PoseStamped
# A Pose with reference coordinate frame and timestamp
Header header
Pose pose

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w

================================================================================
MSG: geometry_msgs/TwistStamped
# A twist with reference coordinate frame and timestamp
Header header
Twist twist

================================================================================
MSG: geometry_msgs/Twist
# This expresses velocity in free space broken into its linear and angular parts.
Vector3  linear
Vector3  angular

================================================================================
MSG: geometry_msgs/Vector3
# This represents a vector in free space. 
# It is only meant to represent a direction. Therefore, it does not
# make sense to apply a translation to it (e.g., when applying a 
# generic rigid transformation to a Vector3, tf2 will only apply the
# rotation). If you want your data to be translatable too, use the
# geometry_msgs/Point message instead.

float64 x
float64 y
float64 z"""
  __slots__ = ['outcome','message','dist_to_goal','angle_to_goal','current_pose','last_cmd_vel']
  _slot_types = ['uint32','string','float32','float32','geometry_msgs/PoseStamped','geometry_msgs/TwistStamped']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       outcome,message,dist_to_goal,angle_to_goal,current_pose,last_cmd_vel

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(MoveBaseFeedback, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.outcome is None:
        self.outcome = 0
      if self.message is None:
        self.message = ''
      if self.dist_to_goal is None:
        self.dist_to_goal = 0.
      if self.angle_to_goal is None:
        self.angle_to_goal = 0.
      if self.current_pose is None:
        self.current_pose = geometry_msgs.msg.PoseStamped()
      if self.last_cmd_vel is None:
        self.last_cmd_vel = geometry_msgs.msg.TwistStamped()
    else:
      self.outcome = 0
      self.message = ''
      self.dist_to_goal = 0.
      self.angle_to_goal = 0.
      self.current_pose = geometry_msgs.msg.PoseStamped()
      self.last_cmd_vel = geometry_msgs.msg.TwistStamped()

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      buff.write(_get_struct_I().pack(self.outcome))
      _x = self.message
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_2f3I().pack(_x.dist_to_goal, _x.angle_to_goal, _x.current_pose.header.seq, _x.current_pose.header.stamp.secs, _x.current_pose.header.stamp.nsecs))
      _x = self.current_pose.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_7d3I().pack(_x.current_pose.pose.position.x, _x.current_pose.pose.position.y, _x.current_pose.pose.position.z, _x.current_pose.pose.orientation.x, _x.current_pose.pose.orientation.y, _x.current_pose.pose.orientation.z, _x.current_pose.pose.orientation.w, _x.last_cmd_vel.header.seq, _x.last_cmd_vel.header.stamp.secs, _x.last_cmd_vel.header.stamp.nsecs))
      _x = self.last_cmd_vel.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_6d().pack(_x.last_cmd_vel.twist.linear.x, _x.last_cmd_vel.twist.linear.y, _x.last_cmd_vel.twist.linear.z, _x.last_cmd_vel.twist.angular.x, _x.last_cmd_vel.twist.angular.y, _x.last_cmd_vel.twist.angular.z))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.current_pose is None:
        self.current_pose = geometry_msgs.msg.PoseStamped()
      if self.last_cmd_vel is None:
        self.last_cmd_vel = geometry_msgs.msg.TwistStamped()
      end = 0
      start = end
      end += 4
      (self.outcome,) = _get_struct_I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.message = str[start:end].decode('utf-8')
      else:
        self.message = str[start:end]
      _x = self
      start = end
      end += 20
      (_x.dist_to_goal, _x.angle_to_goal, _x.current_pose.header.seq, _x.current_pose.header.stamp.secs, _x.current_pose.header.stamp.nsecs,) = _get_struct_2f3I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.current_pose.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.current_pose.header.frame_id = str[start:end]
      _x = self
      start = end
      end += 68
      (_x.current_pose.pose.position.x, _x.current_pose.pose.position.y, _x.current_pose.pose.position.z, _x.current_pose.pose.orientation.x, _x.current_pose.pose.orientation.y, _x.current_pose.pose.orientation.z, _x.current_pose.pose.orientation.w, _x.last_cmd_vel.header.seq, _x.last_cmd_vel.header.stamp.secs, _x.last_cmd_vel.header.stamp.nsecs,) = _get_struct_7d3I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.last_cmd_vel.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.last_cmd_vel.header.frame_id = str[start:end]
      _x = self
      start = end
      end += 48
      (_x.last_cmd_vel.twist.linear.x, _x.last_cmd_vel.twist.linear.y, _x.last_cmd_vel.twist.linear.z, _x.last_cmd_vel.twist.angular.x, _x.last_cmd_vel.twist.angular.y, _x.last_cmd_vel.twist.angular.z,) = _get_struct_6d().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      buff.write(_get_struct_I().pack(self.outcome))
      _x = self.message
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_2f3I().pack(_x.dist_to_goal, _x.angle_to_goal, _x.current_pose.header.seq, _x.current_pose.header.stamp.secs, _x.current_pose.header.stamp.nsecs))
      _x = self.current_pose.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_7d3I().pack(_x.current_pose.pose.position.x, _x.current_pose.pose.position.y, _x.current_pose.pose.position.z, _x.current_pose.pose.orientation.x, _x.current_pose.pose.orientation.y, _x.current_pose.pose.orientation.z, _x.current_pose.pose.orientation.w, _x.last_cmd_vel.header.seq, _x.last_cmd_vel.header.stamp.secs, _x.last_cmd_vel.header.stamp.nsecs))
      _x = self.last_cmd_vel.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      _x = self
      buff.write(_get_struct_6d().pack(_x.last_cmd_vel.twist.linear.x, _x.last_cmd_vel.twist.linear.y, _x.last_cmd_vel.twist.linear.z, _x.last_cmd_vel.twist.angular.x, _x.last_cmd_vel.twist.angular.y, _x.last_cmd_vel.twist.angular.z))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.current_pose is None:
        self.current_pose = geometry_msgs.msg.PoseStamped()
      if self.last_cmd_vel is None:
        self.last_cmd_vel = geometry_msgs.msg.TwistStamped()
      end = 0
      start = end
      end += 4
      (self.outcome,) = _get_struct_I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.message = str[start:end].decode('utf-8')
      else:
        self.message = str[start:end]
      _x = self
      start = end
      end += 20
      (_x.dist_to_goal, _x.angle_to_goal, _x.current_pose.header.seq, _x.current_pose.header.stamp.secs, _x.current_pose.header.stamp.nsecs,) = _get_struct_2f3I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.current_pose.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.current_pose.header.frame_id = str[start:end]
      _x = self
      start = end
      end += 68
      (_x.current_pose.pose.position.x, _x.current_pose.pose.position.y, _x.current_pose.pose.position.z, _x.current_pose.pose.orientation.x, _x.current_pose.pose.orientation.y, _x.current_pose.pose.orientation.z, _x.current_pose.pose.orientation.w, _x.last_cmd_vel.header.seq, _x.last_cmd_vel.header.stamp.secs, _x.last_cmd_vel.header.stamp.nsecs,) = _get_struct_7d3I().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.last_cmd_vel.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.last_cmd_vel.header.frame_id = str[start:end]
      _x = self
      start = end
      end += 48
      (_x.last_cmd_vel.twist.linear.x, _x.last_cmd_vel.twist.linear.y, _x.last_cmd_vel.twist.linear.z, _x.last_cmd_vel.twist.angular.x, _x.last_cmd_vel.twist.angular.y, _x.last_cmd_vel.twist.angular.z,) = _get_struct_6d().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_6d = None
def _get_struct_6d():
    global _struct_6d
    if _struct_6d is None:
        _struct_6d = struct.Struct("<6d")
    return _struct_6d
_struct_2f3I = None
def _get_struct_2f3I():
    global _struct_2f3I
    if _struct_2f3I is None:
        _struct_2f3I = struct.Struct("<2f3I")
    return _struct_2f3I
_struct_7d3I = None
def _get_struct_7d3I():
    global _struct_7d3I
    if _struct_7d3I is None:
        _struct_7d3I = struct.Struct("<7d3I")
    return _struct_7d3I
