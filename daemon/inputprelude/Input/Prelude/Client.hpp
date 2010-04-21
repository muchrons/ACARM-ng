/*
 * Client.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE

#include <string>
#include <prelude-client.h>

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

  /**
   * \brief Initialize and start prelude client
   */
  void start();

  /**
   * \brief Receive IDMEF (either Alert or Heartbeat)
   * \param timeout timeout
   * \return IDMEF message or NULL on timeout
   */
  idmef_message_t* recvMessage(int timeout=-1);

private:
  prelude_client_t         *client_;
  prelude_client_profile_t *profile_;
}; // class Client

} // namespace Prelude
} // namespace Input

#endif
