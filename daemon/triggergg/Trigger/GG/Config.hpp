/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_GG_CONFIG_HPP_FILE

#include "Trigger/GG/AccountConfig.hpp"

namespace Trigger
{
namespace GG
{

// TODO: comment this code

class Config
{
public:
  Config(const AccountConfig &ac, const double severityThreshold
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
