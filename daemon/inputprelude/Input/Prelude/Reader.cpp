/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "Input/Prelude/Client.hpp"
#include "Persistency/Alert.hpp"
#include "Input/Prelude/Reader.hpp"
#include "Input/Prelude/IDMEFParser.hpp"

namespace Input
{
namespace Prelude
{

  Reader::Reader(const std::string profile, const std::string config):
  Input::Reader("profile"),
  preludeLogger_("input.prelude.reader.preludelog"),
  preludeProfile_(profile),
  client_( new Client( preludeProfile_.c_str() ) )
{
  assert( client_.get()!=NULL );

  client_->SetConfigFilename(config.c_str());
  client_->SetRequiredPermission(PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  client_->start();
}

Reader::DataPtr Reader::read(const unsigned int timeout)
{
  DataPtr tmp;
  assert(tmp.get()==NULL);

  idmef_message_t * message=NULL;

  message=client_->recvMessage(timeout);

  //in case of timeout 'message' equals to null
  if (!message)
    return tmp;

  IDMEFParser ip(message);

  tmp.reset(new Persistency::Alert(ip.getName(),ip.getAnalyzers(),NULL,ip.getCreateTime(),Persistency::Severity(Persistency::SeverityLevel::DEBUG),Persistency::Certainty(1.0),"",ip.getSources(),ip.getTargets()));

  return tmp;
}

} // namespace Prelude
} // namespace Input
