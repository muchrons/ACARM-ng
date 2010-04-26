/*
 * Client.cpp
 *
 */

#include "Input/Prelude/Client.hpp"

namespace Input
{
namespace Prelude
{

Client::Client(const std::string& profile, const std::string& config, int permission)
{
  int argc=1;
  char a1[]="name";
  char *argv[]={a1};
  if ( prelude_init(&argc, argv) < 0 )
    throw Input::Exception(SYSTEM_SAVE_LOCATION, "Unable to initialize prelude library.");

  if ( prelude_client_new(&client_, profile.c_str()) < 0 )
      throw Exception(SYSTEM_SAVE_LOCATION, "Cannot create prelude client.");

  profile_ = prelude_client_get_profile(client_);

  if ( prelude_client_set_config_filename(client_, config.c_str()) < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error reading prelude config file " + config);

  prelude_client_set_required_permission(client_, (prelude_connection_permission_t) permission);
}

Client::~Client()
{
  profile_ = NULL;
  prelude_client_destroy(client_, PRELUDE_CLIENT_EXIT_STATUS_SUCCESS);
  //prelude_deinit();
}

void Client::start()
{
  int ret = prelude_client_init(client_);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot initialize prelude client.");

  profile_ = prelude_client_get_profile(client_);

  ret = prelude_client_start(client_);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot start prelude client.");
}

idmef_message_t* Client::recvMessage(int timeout)
{
  idmef_message_t *idmef_p;

  const int ret = prelude_client_recv_idmef(client_, timeout, &idmef_p);
  if ( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error while receiving IDMEF.");
  else if ( ret == 0 )
    return NULL;

  return idmef_p;
}

} // namespace Prelude
} // namespace Input


