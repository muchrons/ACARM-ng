/*
 * Connection.cpp
 *
 */
#include "Trigger/GG/Connection.hpp"

namespace Trigger
{
namespace GG
{

Connection::Connection(const AccountConfig &cfg):
  params_(cfg),
  sess_( gg_login( &params_.get() ) )
{
  if( sess_.get()==NULL )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "unable to connect/login to server");
}

} // namespace GG
} // namespace Trigger
