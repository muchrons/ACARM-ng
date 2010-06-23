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
  if ( prelude_client_new(&client_, profile.c_str()) < 0 )
      throw Exception(SYSTEM_SAVE_LOCATION, "Cannot create prelude client.");

  if ( prelude_client_set_config_filename(client_, config.c_str()) < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error reading prelude config file " + config);

  prelude_client_set_required_permission(client_, (prelude_connection_permission_t) permission);

  // TODO: consider making 2nd parameter temporary, constant to increase readability
  if ( prelude_client_set_flags(client_, static_cast<prelude_client_flags_t>(prelude_client_get_flags(client_) | PRELUDE_CLIENT_FLAGS_ASYNC_TIMER)) < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot set ASYNC TIMER.");
  // TODO: do not keep dead code in sources - use VCS instead.
  /*
  prelude_client_profile_set_uid(prelude_client_get_profile(client_),getuid());
  prelude_client_profile_set_gid(prelude_client_get_profile(client_),getgid());
  */
  if ( prelude_client_init(client_) < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot initialize prelude client.");

  if ( prelude_client_start(client_) < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Cannot start prelude client.");
}

Client::~Client()
{
  prelude_client_destroy(client_, PRELUDE_CLIENT_EXIT_STATUS_SUCCESS);
}

Client::IdmefMessageAutoPtr Client::recvMessage(int timeout)
{
  idmef_message_t *idmef_p=NULL;

  const int ret = prelude_client_recv_idmef(client_, timeout, &idmef_p);
  if( ret < 0 )
    throw Exception(SYSTEM_SAVE_LOCATION, "Error while receiving IDMEF.");
  if( ret == 0 )
    return IdmefMessageAutoPtr();

  assert(idmef_p!=NULL);

  return IdmefMessageAutoPtr(idmef_p);
}

} // namespace Prelude
} // namespace Input


