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

// TODO: const references should be taken as arguments here
Reader::Reader(const std::string profile, const std::string config):
  Input::Reader(profile),
  // TODO: logger is not used here
  preludeLogger_("input.prelude.reader.preludelog"),
  client_( new Client(profile, config, PRELUDE_CONNECTION_PERMISSION_IDMEF_READ) )
{
  assert( client_.get()!=NULL );
  client_->start();
}

Reader::DataPtr Reader::read(const unsigned int timeout)
{
  DataPtr tmp;
  assert(tmp.get()==NULL);

  idmef_message_t * message=NULL;
  message=client_->recvMessage(timeout);

  // in case of timeout 'message' equals to null
  if (!message)
    return tmp;

  // TODO: this variable should be const
  IDMEFParser ip(message);
  tmp.reset(new Persistency::Alert(ip.getName(),
                                   ip.getAnalyzers(),
                                   NULL,
                                   ip.getCreateTime(),
                                   Persistency::Severity(Persistency::SeverityLevel::DEBUG),    // TODO
                                   Persistency::Certainty(1.0),
                                   "",
                                   ip.getSources(),
                                   ip.getTargets()));
  return tmp;
}

} // namespace Prelude
} // namespace Input
