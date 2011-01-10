/*
 * LoggerNodes.cpp
 *
 */
#include <cassert>

#include "ConfigIO/LoggerNodes.hpp"

using namespace std;

namespace ConfigIO
{

LoggerNodes::LoggerNodes(const Nodes &ns):
  ns_(ns)
{
  // for each node
  for(const_iterator it1=begin(); it1!=end(); ++it1)
  {
    // check number of instances
    int cnt=0;
    for(const_iterator it2=begin(); it2!=end(); ++it2)
      if( it1->getNodeName()==it2->getNodeName() )
        ++cnt;

    // raise exception when entries are not unique
    assert(cnt!=0);
    if(cnt!=1)
      throw ExceptionDuplicatedNodeName(SYSTEM_SAVE_LOCATION, it1->getNodeName() );
  }
}

} // namespace ConfigIO
