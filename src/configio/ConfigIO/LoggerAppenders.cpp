/*
 * LoggerAppenders.cpp
 *
 */
#include <map>
#include <cassert>

#include "ConfigIO/LoggerAppenders.hpp"

using namespace std;

namespace ConfigIO
{

LoggerAppenders::LoggerAppenders(const Appenders &apps):
  apps_(apps)
{
  // for each appender
  for(const_iterator it1=begin(); it1!=end(); ++it1)
  {
    // check number of instances
    int cnt=0;
    for(const_iterator it2=begin(); it2!=end(); ++it2)
      if( it1->getName()==it2->getName() )
        ++cnt;

    // raise exception when entries are not unique
    assert(cnt!=0);
    if(cnt!=1)
      throw ExceptionInvalidDuplicatedAppenderName(
                    "LoggerAppenders::LoggerAppenders()",
                    it1->getName() );
  }
}

} // namespace ConfigIO
