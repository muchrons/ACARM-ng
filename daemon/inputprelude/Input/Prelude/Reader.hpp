/*
 * Reader.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_READER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_READER_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>
#include <PreludePP/idmef.hpp>
#include <PreludePP/prelude-client.hpp>

#include "Input/Reader.hpp"



// TODO: implement
// TODO: test

namespace Input
{
namespace Prelude
{

/** \brief generic input interface class.
 */
class Reader: public Input::Reader
{
public:
  Reader(const std::string profile)
  {
    prelude_profile=profile;
    client.reset(new ::Prelude::Client("ACARM-wojek-acm"));
    client->SetConfigFilename("/etc/prelude/default/client.conf");
    client->SetRequiredPermission(PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
    //client.SetFlags( Client::FLAGS_ASYNC_TIMER);
    client->Init();
    client->Start();    
  }

public:
  /** \brief reads data from input from prelude-manager.
   *  \param timeout milliseconds to wait before timeout occures.
   *  \return pointer to newly allocated strucutre or NULL on timeout.
   */
  virtual DataPtr read(unsigned int timeout);

private:
  std::string prelude_profile;
  std::auto_ptr< ::Prelude::Client > client;
}; // class Reader

} // namespace Prelude
} // namespace Input

#endif
