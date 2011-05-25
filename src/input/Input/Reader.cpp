/*
 * Reader.cpp
 *
 */
#include "Input/Reader.hpp"

namespace Input
{

Reader::~Reader(void)
{
  LOGMSG_INFO(log_, "quiting");
}

Reader::Reader(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name):
  log_( Logger::NodeName( "input.reader", Logger::NodeName::removeInvalidChars(type.str() + "." + name.str()).c_str() ) ),
  type_(type),
  name_(name)
{
  LOGMSG_INFO(log_, "creating");
}

} // namespace Input
