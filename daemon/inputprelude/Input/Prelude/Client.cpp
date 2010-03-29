/*
 * Client.cpp
 *
 */

#include "Input/Exception.hpp"
#include "Input/Prelude/Client.hpp"

namespace Input
{
namespace Prelude
{

Client::Client(const char *profile)
{
  const int ret = prelude_client_new(&_client, profile);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot create prelude client.");
  
  _profile = prelude_client_get_profile(_client);
}

Client::~Client()
{
  _profile = NULL;
  prelude_client_destroy(_client, PRELUDE_CLIENT_EXIT_STATUS_SUCCESS);
}

void Client::start()
{
  int ret = prelude_client_init(_client);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot initialize prelude client.");
  
  _profile = prelude_client_get_profile(_client);
  
  ret = prelude_client_start(_client);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot start prelude client.");
}

idmef_message_t* Client::recvMessage(int timeout)
{
  idmef_message_t *idmef_p;

  const int ret = prelude_client_recv_idmef(_client, timeout, &idmef_p);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error while receiving IDMEF.");
  else if ( ret == 0 )
    return NULL;
  
  return idmef_p;
}

void Client::SetConfigFilename(const char *name)
{
  const int ret = prelude_client_set_config_filename(_client, name);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error reading prelude config file " + std::string(name));
}

void Client::SetRequiredPermission(int permission)
{
  prelude_client_set_required_permission(_client, (prelude_connection_permission_t) permission);
}
  

} // namespace Prelude
} // namespace Input


