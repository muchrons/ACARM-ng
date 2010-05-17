/*
 * AccountConfig.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_ACCOUNTCONFIG_HPP_FILE
#define INCLUDE_TRIGGER_GG_ACCOUNTCONFIG_HPP_FILE

#include <string>
#include <cinttypes>

#include "Trigger/GG/UserID.hpp"

namespace Trigger
{
namespace GG
{

// TODO: comment this code

class AccountConfig
{
public:
  AccountConfig(UserID uid, const std::string &password):
    uid_(uid),
    password_(password)
  {
  }

  UserID getUserID(void) const
  {
    return uid_;
  }

  const std::string &getPassword(void) const
  {
    return password_;
  }

private:
  UserID      uid_;
  std::string password_;
}; // class AccountConfig

} // namespace GG
} // namespace Trigger

#endif
