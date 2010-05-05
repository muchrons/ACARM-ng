/*
 * Reader.cpp
 *
 */
#include <sstream>
#include <ctype.h>

#include "Input/Reader.hpp"

using namespace std;

namespace Input
{

Reader::~Reader(void)
{
  LOGMSG_INFO(log_, "quiting");
}

namespace
{
// helper function to leave only characters that are allowed for logger name
string makeValidString(const string &name)
{
  stringstream ss;
  // allow only basic chars - skip the rest
  for(string::const_iterator it=name.begin(); it!=name.end(); ++it)
    if( isdigit(*it) || islower(*it) )
      ss<<*it;
  return ss.str();
} // makeValieString()
} // unnamed namespace

Reader::Reader(const std::string &name):
  log_( Logger::NodeName( "input.reader", makeValidString(name).c_str() ) ),
  name_(name)
{
  LOGMSG_INFO(log_, "creating");
}

} // namespace Input
