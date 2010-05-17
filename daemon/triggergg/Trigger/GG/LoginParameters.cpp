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

LoginParameters::LoginParameters(const AccountConfig &cfg)
{
  // zero whole structure
  memset(&p_, 0, sizeof(p_) );

  // save password in editable form (GG does not have const here)
  const std::string &pass=cfg.getPassword();
  const size_t       len =pass.length();
  pass_.reset(new char[len+1]);
  strcpy(pass_.get(), pass.c_str() );
  assert( strlen( pass_.get() )==len ); // ensure NULL-termination
  assert( pass==pass_.get() );          // ensure identical content

  // set explicitly required parameters
  p_.uin      =cfg.getUserID();
  p_.password =pass_.get();
  p_.hash_type=GG_LOGIN_HASH_SHA1;
  p_.encoding =GG_ENCODING_UTF8;
}

} // namespace GG
} // namespace Trigger
