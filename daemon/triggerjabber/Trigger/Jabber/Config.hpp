/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_CONFIG_HPP_FILE

#include "Trigger/Simple/ThresholdConfig.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"

namespace Trigger
{
namespace Jabber
{

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  // TODO: 'receiver' should be const-ref.
  /** \brief create configration description.
   *  \param ac       account's config to be used for connection (account must exist).
   *  \param receiver notification receiver.
   *  \param th       threshold configuration - informs when run trigger.
   */
  Config(const AccountConfig &ac, const std::string receiver, const Simple::ThresholdConfig &th):
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
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }
  // TODO: const-ref should be returned
  /** \brief gets receiver's data.
   *  \return receiver's name.
   */
  const std::string getReceiver(void) const
  {
    return receiver_;
  }

private:
  AccountConfig           ac_;
  Simple::ThresholdConfig th_;
  std::string             receiver_;
}; // class Config

} // namespace Jabber
} // namespace Trigger

#endif