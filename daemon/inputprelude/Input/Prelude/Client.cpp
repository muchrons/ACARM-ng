/*
 * Client.cpp
 *
 */
#include <iostream> // TODO: remove this header - libs are not allowsd to output anything on the screen
#include "Input/Prelude/Client.hpp"

namespace Input
{
namespace Prelude
{

Client::Client(const std::string& profile, const std::string& config, int permission)
{
  if ( prelude_client_new(&client_, profile.c_str()) < 0 )
      throw Exception(SYSTEM_SAVE_LOCATION, "Cannot create prelude client.");

  // TODO: this variable is nevere read from
  profile_ = prelude_client_get_profile(client_);
  // TODO: assert on non-null pointer here

  if ( prelude_client_set_config_filename(client_, config.c_str()) < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error reading prelude config file " + config);

  prelude_client_set_required_permission(client_, (prelude_connection_permission_t) permission);
}

Client::~Client()
{
  profile_ = NULL;
  prelude_client_destroy(client_, PRELUDE_CLIENT_EXIT_STATUS_SUCCESS);
}

void Client::start()
{
  int ret = prelude_client_init(client_);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot initialize prelude client.");

  // TODO: the same code happens in c-tor
  profile_ = prelude_client_get_profile(client_);
  // TODO: assert on non-null pointer here

  ret = prelude_client_start(client_);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot start prelude client.");
}

idmef_message_t* Client::recvMessage(int timeout)
{
  // TODO: i'd suggest NULL-initialize this pointer - just in case...
  idmef_message_t *idmef_p;

  const int ret = prelude_client_recv_idmef(client_, timeout, &idmef_p);
  if( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error while receiving IDMEF.");
  if( ret == 0 )
    return NULL;

  // TODO: assert idmef_p!=NULL whould be nice here
  return idmef_p;
}

} // namespace Prelude
} // namespace Input


