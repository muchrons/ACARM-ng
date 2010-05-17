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

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  /** \brief create configration description.
   *  \param ac       account's config to be used for connection (account must exist).
   *  \param receiver notification receiver.
   *  \param th       threshold configuration - informs when run trigger.
   */
  Config(const AccountConfig &ac, const UserID receiver, const Simple::ThresholdConfig &th):
    ac_(ac),
    th_(th),
    receiver_(receiver)
  {
  }

  /** \brief get configuration for account.
   *  \return account's config.
   */
  const AccountConfig &getAccountConfig(void) const
  {
    return ac_;
  }
  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig getThresholdConfig(void) const
  {
    return th_;
  }
  /** \brief gets receiver's data.
   *  \return receiver's UID.
   */
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
