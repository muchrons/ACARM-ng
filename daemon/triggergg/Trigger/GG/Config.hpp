/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_GG_CONFIG_HPP_FILE

#include "Trigger/Simple/ThresholdConfig.hpp"
#include "Trigger/GG/UserID.hpp"
#include "Trigger/GG/AccountConfig.hpp"

namespace Trigger
{
namespace GG
{

// TODO: comment this code

class Config
{
public:
  Config(const AccountConfig &ac, const UserID receiver, const Simple::ThresholdConfig &th):
    ac_(ac),
    th_(th),
    receiver_(receiver)
  {
  }

  const AccountConfig &getAccountConfig(void) const
  {
    return ac_;
  }

  const Simple::ThresholdConfig getThresholdConfig(void) const
  {
    return th_;
  }

  UserID getReceiver(void) const
  {
    return receiver_;
  }

private:
  AccountConfig           ac_;
  Simple::ThresholdConfig th_;
  UserID                  receiver_;
}; // class AccountConfig

} // namespace GG
} // namespace Trigger

#endif
