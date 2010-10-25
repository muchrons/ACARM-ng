/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_GG_CONFIG_HPP_FILE

#include "Base/NonEmptyVector.hpp"
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
  /** \brief list of receivers. */
  typedef Base::NonEmptyVector<UserID> Receivers;

  /** \brief create configration description.
   *  \param ac       account's config to be used for connection (account must exist).
   *  \param receiver notification receiver.
   *  \param th       threshold configuration - informs when run trigger.
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
  /** \brief gets receivers' data.
   *  \return receiver's UID list.
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

} // namespace GG
} // namespace Trigger

#endif
