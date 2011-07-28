/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "Input/Prelude/Client.hpp"
#include "Persistency/Alert.hpp"
#include "Input/Prelude/Reader.hpp"
#include "Input/Prelude/IDMEFParser.hpp"
#include "Input/Prelude/IDMEFParserAlert.hpp"
#include "Input/Prelude/IDMEFParserHeartbeat.hpp"
#include "System/AutoVariable.hpp"
#include "Input/Prelude/detail/IdmefMessageHolder.hpp"

using namespace Core::Types::Proc;

namespace Input
{
namespace Prelude
{

Reader::Reader(const std::string &profile, const Core::Types::Proc::InstanceName &name, const std::string &config):
  Input::Reader(TypeName("prelude"), name),
  client_( new Client(profile, config, PRELUDE_CONNECTION_PERMISSION_IDMEF_READ) )
{
  assert( client_.get()!=NULL );
}

Reader::DataPtr Reader::read(BackendFacade &bf, const unsigned int timeout)
{
  DataPtr tmp;
  assert(tmp.get()==NULL);

  System::AutoVariable<detail::IdmefMessageHolder> message( client_->recvMessage(timeout) );

  // in case of timeout 'message' equals to null
  if (!message.get())
    return tmp;

  const unsigned int heartbeat_timeout=10; //todo: remove
  try
  {
    if( IDMEFParser::isAlert( message.get()) )
      {
        const IDMEFParserAlert ipa( message.get(), bf );
        tmp.reset(new Persistency::Alert(ipa.getName(),
                                         ipa.getAnalyzers(),
                                         NULL,
                                         ipa.getCreateTime(),
                                         ipa.getSeverity(),
                                         Persistency::Certainty(1.0),
                                         ipa.getDescription(),
                                         ipa.getSources(),
                                         ipa.getTargets()));
      }
    else
      {
        if ( IDMEFParser::isHeartbeat( message.get()) )
          IDMEFParserHeartbeat( message.get(), bf, heartbeat_timeout );
        else
          throw ExceptionUnsupportedFeature(SYSTEM_SAVE_LOCATION,"Unknown message type received.");
      }
  }
  catch(const ExceptionUnsupportedFeature &ex)
  {
    LOGMSG_DEBUG_S(log_)<<"Exception upon unsupported feature request: "<<ex.what();
    return tmp;
  }
  catch(const ExceptionHeartbeat &)
  {
    //no alert is returned in case of a heartbeat
    return tmp;
  }

  return tmp;
}

} // namespace Prelude
} // namespace Input
