/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "PreludePP/idmef.hpp"
#include "PreludePP/prelude-client.hpp"
#include "Persistency/Alert.hpp"
#include "Input/Prelude/Reader.hpp"

namespace Input
{
namespace Prelude
{

Reader::Reader(const std::string profile):
  preludeLogger_("input.prelude.reader.preludelog"),
  preludeProfile_(profile),
  client_( new ::Prelude::Client( preludeProfile_.c_str() ) )
{
  assert( client_.get()!=NULL );
  client_->SetConfigFilename("/etc/prelude/default/client.conf");
  client_->SetRequiredPermission(PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  //client.SetFlags( Client::FLAGS_ASYNC_TIMER);
  client_->Init();
  client_->Start();
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
