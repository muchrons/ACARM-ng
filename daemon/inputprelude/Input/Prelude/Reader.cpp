/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "Input/Prelude/Reader.hpp"
#include "Persistency/Alert.hpp"

#include <idmef.hxx>                                                                                                                         
#include <prelude-client.hxx>

namespace Input
{
namespace Prelude
{

Reader::Reader(const std::string profile):
  preludeLogger_("input.prelude.reader.preludelog"),
  prelude_profile(profile),
  client(new ::Prelude::Client("ACARM-wojek-acm")) 
{
    client->SetConfigFilename("/etc/prelude/default/client.conf");
    client->SetRequiredPermission(PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
    //client.SetFlags( Client::FLAGS_ASYNC_TIMER);
    client->Init();
    client->Start();    
}

Reader::DataPtr Reader::read(const unsigned int /*timeout*/)
{
  // TODO: implement
  DataPtr tmp;
  assert(tmp.get()==NULL);

  //IDMEF idmef;
  //client >> idmef;

  
  
  
  return tmp;//new Alert();
}

} // namespace Prelude
} // namespace Input
