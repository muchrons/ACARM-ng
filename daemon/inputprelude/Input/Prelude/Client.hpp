/*
 * Client.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE

#include <string>
// TODO: include "prelude..."
#include <prelude-client.h>
#include <prelude.h>
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
class Client
{
public:
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

  // TODO: why this is not done in c-tor? and if not, why there is no stop() method?
  /**
   * \brief Initialize and start prelude client
   */
  void start();

  // TODO: returned value should be wrapped into auto_ptr<> like manier, to prevent
  //       resource leaks on exceptions. notice: you can customize System::AutoVariable<>
  //       template for this task.
  /**
   * \brief Receive IDMEF (either Alert or Heartbeat)
   * \param timeout timeout
   * \return IDMEF message or NULL on timeout
   */
  idmef_message_t* recvMessage(int timeout=-1);

private:
  GlobalLibPreludeInit      g_;
  LogCallback               preludeLogger_;
  prelude_client_t         *client_;
  prelude_client_profile_t *profile_;   // TODO: unused variable
}; // class Client

} // namespace Prelude
} // namespace Input

#endif
