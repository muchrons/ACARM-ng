/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_CONFIG_HPP_FILE

#include "Base/NonEmptyVector.hpp"
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
  /** \brief list of receivers. */
  typedef Base::NonEmptyVector<std::string> Receivers;
  /** \brief create configration description.
   *  \param ac        account's config to be used for connection (account must exist).
   *  \param receivers list of accounts that are to receive notifications.
   *  \param th        threshold configuration - informs when run trigger.
   */
  Config(const AccountConfig &ac, const Receivers &receivers, const Simple::ThresholdConfig &th):
    ac_(ac),
    th_(th),
    receivers_(receivers)
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
  /** \brief gets receiver's data.
   *  \return receiver's name.
   */
  const Receivers &getReceivers(void) const
  {
    return receivers_;
  }

private:
  AccountConfig           ac_;
  Simple::ThresholdConfig th_;
  Receivers               receivers_;
}; // class Config

} // namespace Jabber
} // namespace Trigger

#endif
