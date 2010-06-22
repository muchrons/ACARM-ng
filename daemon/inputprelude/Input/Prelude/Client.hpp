/*
 * Client.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>
#include <prelude.h>
#include <prelude-client.h>

#include "System/AutoVariable.hpp"
#include "Input/Prelude/detail/IdmefMessageHolder.hpp"
#include "Input/Prelude/GlobalLibPreludeInit.hpp"
#include "Input/Prelude/LogCallback.hpp"
#include "Input/Exception.hpp"

namespace Input
{
namespace Prelude
{

/**
 * \brief prelude_client_t wrapper
 */
class Client: private boost::noncopyable
{
public:
  /**
   * Auto variable to deallocate idmef_message_t in case of exception
   */
  typedef System::AutoVariable<detail::IdmefMessageHolder> IdmefMessageAutoPtr;

  /**
   * \brief c-tor, creates a prelude client with given profile.
   * \param profile profile to be used.
   * \param config path to a config file.
   * \param permission permission to acquire
   */
  Client(const std::string &profile, const std::string &config, int permission);

  /**
   * \brief d-tor
   */
  ~Client();

  /**
   * \brief Receive IDMEF (either Alert or Heartbeat)
   * \param timeout timeout
   * \return IDMEF message or NULL on timeout
   */
  IdmefMessageAutoPtr recvMessage(int timeout=-1);

private:
  GlobalLibPreludeInit      g_;
  LogCallback               preludeLogger_;
  prelude_client_t         *client_;
}; // class Client

} // namespace Prelude
} // namespace Input

#endif
