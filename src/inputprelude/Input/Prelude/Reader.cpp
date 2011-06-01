/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "Input/Prelude/Client.hpp"
#include "Persistency/Alert.hpp"
#include "Input/Prelude/Reader.hpp"
#include "Input/Prelude/IDMEFParser.hpp"
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

  try
  {
    const IDMEFParser ip( message.get(), bf );
    tmp.reset(new Persistency::Alert(ip.getName(),
                                     ip.getAnalyzers(),
                                     NULL,
                                     ip.getCreateTime(),
                                     ip.getSeverity(),
                                     Persistency::Certainty(1.0),
                                     ip.getDescription(),
                                     ip.getSources(),
                                     ip.getTargets()));
  }
  catch(const ExceptionUnsupportedFeature &ex)
  {
    LOGMSG_DEBUG_S(log_)<<"exception uppon unsupported feature request: "<<ex.what();
    // we can ignore this and return NULL
    assert( tmp.get()==NULL );
  }

  return tmp;
}

} // namespace Prelude
} // namespace Input
