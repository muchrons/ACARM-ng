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

Reader::Reader(const std::string &name):
  log_( Logger::NodeName( "input.reader",
                          Logger::NodeName::removeInvalidChars(name).c_str() ) ),
  name_(name)
{
  LOGMSG_INFO(log_, "creating");
}

} // namespace Input
