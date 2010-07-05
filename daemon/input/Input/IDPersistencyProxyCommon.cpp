/*
 * IDPersistencyProxyCommon.cpp
 *
 */
#include <boost/lexical_cast.hpp>

#include "Input/IDPersistencyProxyCommon.hpp"

using namespace std;
using namespace Persistency::IO;

namespace Input
{
namespace
{
const char *nextFreeIDParam="next free ID";
} // unnamed namespace

IDPersistencyProxyCommon::IDPersistencyProxyCommon(Persistency::IO::ConnectionPtrNN  conn,
                                                   Persistency::IO::Transaction     &t):
  detail::IDPersistencyProxyBase("", conn, t)
{
}

Persistency::Analyzer::ID IDPersistencyProxyCommon::readNextFreeID(void)
{
  DynamicConfig::ValueNULL tmp=dynConf_.read(nextFreeIDParam);
  // no value set? assume counter has ust started.
  if( tmp.get()==NULL )
    return 0;
  // parse input value and return to caller
  const Persistency::Analyzer::ID out=boost::lexical_cast<Persistency::Analyzer::ID>( tmp.get()->get() );
  return out;
}

void IDPersistencyProxyCommon::saveNextFreeID(Persistency::Analyzer::ID nextFreeID)
{
  dynConf_.write( nextFreeIDParam, boost::lexical_cast<string>(nextFreeID) );
}

} // namespace Input
