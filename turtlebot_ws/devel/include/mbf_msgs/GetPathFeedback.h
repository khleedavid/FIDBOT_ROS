// Generated by gencpp from file mbf_msgs/GetPathFeedback.msg
// DO NOT EDIT!


#ifndef MBF_MSGS_MESSAGE_GETPATHFEEDBACK_H
#define MBF_MSGS_MESSAGE_GETPATHFEEDBACK_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace mbf_msgs
{
template <class ContainerAllocator>
struct GetPathFeedback_
{
  typedef GetPathFeedback_<ContainerAllocator> Type;

  GetPathFeedback_()
    {
    }
  GetPathFeedback_(const ContainerAllocator& _alloc)
    {
  (void)_alloc;
    }







  typedef boost::shared_ptr< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> const> ConstPtr;

}; // struct GetPathFeedback_

typedef ::mbf_msgs::GetPathFeedback_<std::allocator<void> > GetPathFeedback;

typedef boost::shared_ptr< ::mbf_msgs::GetPathFeedback > GetPathFeedbackPtr;
typedef boost::shared_ptr< ::mbf_msgs::GetPathFeedback const> GetPathFeedbackConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::mbf_msgs::GetPathFeedback_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace mbf_msgs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'nav_msgs': ['/opt/ros/melodic/share/nav_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/melodic/share/std_msgs/cmake/../msg'], 'actionlib_msgs': ['/opt/ros/melodic/share/actionlib_msgs/cmake/../msg'], 'geometry_msgs': ['/opt/ros/melodic/share/geometry_msgs/cmake/../msg'], 'mbf_msgs': ['/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const ::mbf_msgs::GetPathFeedback_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "mbf_msgs/GetPathFeedback";
  }

  static const char* value(const ::mbf_msgs::GetPathFeedback_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"\n"
;
  }

  static const char* value(const ::mbf_msgs::GetPathFeedback_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream&, T)
    {}

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct GetPathFeedback_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::mbf_msgs::GetPathFeedback_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream&, const std::string&, const ::mbf_msgs::GetPathFeedback_<ContainerAllocator>&)
  {}
};

} // namespace message_operations
} // namespace ros

#endif // MBF_MSGS_MESSAGE_GETPATHFEEDBACK_H
