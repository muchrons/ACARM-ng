/*
 * Client.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_CLIENT_HPP_FILE

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
  /*
   * \brief c-tor, creates a prelude client with given profile.
   * \param profile profile to be used.
   */
  Client(const char *profile);

  /*
   * \brief d-tor
   */
  ~Client();

  /*
   * \brief Set prelude config file.
   * \param name path to a config file.
   */
  void SetConfigFilename(const char *name);

  /*
   * \brief Set required permission to access prelude manager
   * \param permission permission to acquire
   */
  void SetRequiredPermission(int permission);

  /*
   * \brief Initialize and start prelude client
   */
  void start();

  /*
   * \brief Receive IDMEF (either Alert or Heartbeat)
   * \return IDMEF message or NULL on timeout
   */
  idmef_message_t* recvMessage(int timeout=-1);

private:
  prelude_client_t *client_;
  prelude_client_profile_t *profile_;
}; // class Client

} // namespace Prelude
} // namespace Input

#endif
