/*
 * IDPersistencyProxySpecific.cpp
 *
 */
#include "Commons/Convert.hpp"
#include "Input/IDPersistencyProxySpecific.hpp"

using namespace std;
using namespace Persistency::IO;

namespace Input
{

IDPersistencyProxySpecific::IDPersistencyProxySpecific(Persistency::IO::ConnectionPtrNN  conn,
                                                       Persistency::IO::Transaction     &t,
                                                       const std::string                &inputName):
  detail::IDPersistencyProxyBase(inputName, conn, t)
{
}

void IDPersistencyProxySpecific::saveMapping(const std::string &inputID, Persistency::Analyzer::ID id)
{
  dynConf_.write( inputID, Commons::Convert::to<string>( id.get() ) );
}

} // namespace Input
