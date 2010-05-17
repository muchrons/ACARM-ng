/*
 * LoginParameters.cpp
 *
 */
#include <cstring>
#include <cassert>

#include "Trigger/GG/LoginParameters.hpp"

namespace Trigger
{
namespace GG
{

LoginParameters::LoginParameters(const AccountConfig &cfg):
  pass_( cfg.getPassword() )
{
  // zero whole structure
  memset(&p_, 0, sizeof(p_) );

  // set explicitly required parameters
  p_.uin      =cfg.getUserID();
  p_.password =pass_.get();
  p_.hash_type=GG_LOGIN_HASH_SHA1;
  p_.encoding =GG_ENCODING_UTF8;
}

} // namespace GG
} // namespace Trigger
