// Generated by gencpp from file mbf_msgs/ExePathGoal.msg
// DO NOT EDIT!


#ifndef MBF_MSGS_MESSAGE_EXEPATHGOAL_H
#define MBF_MSGS_MESSAGE_EXEPATHGOAL_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <nav_msgs/Path.h>

namespace mbf_msgs
{
template <class ContainerAllocator>
struct ExePathGoal_
{
  typedef ExePathGoal_<ContainerAllocator> Type;

  ExePathGoal_()
    : path()
    , controller()
    , concurrency_slot(0)  {
    }
  ExePathGoal_(const ContainerAllocator& _alloc)
    : path(_alloc)
    , controller(_alloc)
    , concurrency_slot(0)  {
  (void)_alloc;
    }



   typedef  ::nav_msgs::Path_<ContainerAllocator>  _path_type;
  _path_type path;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _controller_type;
  _controller_type controller;

   typedef uint8_t _concurrency_slot_type;
  _concurrency_slot_type concurrency_slot;





  typedef boost::shared_ptr< ::mbf_msgs::ExePathGoal_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::mbf_msgs::ExePathGoal_<ContainerAllocator> const> ConstPtr;

}; // struct ExePathGoal_

typedef ::mbf_msgs::ExePathGoal_<std::allocator<void> > ExePathGoal;

typedef boost::shared_ptr< ::mbf_msgs::ExePathGoal > ExePathGoalPtr;
typedef boost::shared_ptr< ::mbf_msgs::ExePathGoal const> ExePathGoalConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::mbf_msgs::ExePathGoal_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace mbf_msgs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'nav_msgs': ['/opt/ros/melodic/share/nav_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/melodic/share/std_msgs/cmake/../msg'], 'actionlib_msgs': ['/opt/ros/melodic/share/actionlib_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/melodic/share/geometry_msgs/cmake/../msg'], 'mbf_msgs': ['/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::mbf_msgs::ExePathGoal_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::mbf_msgs::ExePathGoal_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::mbf_msgs::ExePathGoal_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "33ff17a095839640ba7e28c0dc858031";
  }

  static const char* value(const ::mbf_msgs::ExePathGoal_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x33ff17a095839640ULL;
  static const uint64_t static_value2 = 0xba7e28c0dc858031ULL;
};

template<class ContainerAllocator>
struct DataType< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "mbf_msgs/ExePathGoal";
  }

  static const char* value(const ::mbf_msgs::ExePathGoal_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"# Follow the given path until completion or failure\n"
"\n"
"nav_msgs/Path path\n"
"\n"
"# Controller to use; defaults to the first one specified on \"controllers\" parameter\n"
"string controller\n"
"\n"
"# use different slots for concurrency\n"
"uint8 concurrency_slot\n"
"\n"
"\n"
"================================================================================\n"
"MSG: nav_msgs/Path\n"
"#An array of poses that represents a Path for a robot to follow\n"
"Header header\n"
"geometry_msgs/PoseStamped[] poses\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
"\n"
"================================================================================\n"
"MSG: geometry_msgs/PoseStamped\n"
"# A Pose with reference coordinate frame and timestamp\n"
"Header header\n"
"Pose pose\n"
"\n"
"================================================================================\n"
"MSG: geometry_msgs/Pose\n"
"# A representation of pose in free space, composed of position and orientation. \n"
"Point position\n"
"Quaternion orientation\n"
"\n"
"================================================================================\n"
"MSG: geometry_msgs/Point\n"
"# This contains the position of a point in free space\n"
"float64 x\n"
"float64 y\n"
"float64 z\n"
"\n"
"================================================================================\n"
"MSG: geometry_msgs/Quaternion\n"
"# This represents an orientation in free space in quaternion form.\n"
"\n"
"float64 x\n"
"float64 y\n"
"float64 z\n"
"float64 w\n"
;
  }

  static const char* value(const ::mbf_msgs::ExePathGoal_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.path);
      stream.next(m.controller);
      stream.next(m.concurrency_slot);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct ExePathGoal_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::mbf_msgs::ExePathGoal_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::mbf_msgs::ExePathGoal_<ContainerAllocator>& v)
  {
    s << indent << "path: ";
    s << std::endl;
    Printer< ::nav_msgs::Path_<ContainerAllocator> >::stream(s, indent + "  ", v.path);
    s << indent << "controller: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.controller);
    s << indent << "concurrency_slot: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.concurrency_slot);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MBF_MSGS_MESSAGE_EXEPATHGOAL_H
