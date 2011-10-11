/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_CONFIG_HPP_SNORTSAM
#define INCLUDE_TRIGGER_SNORTSAM_CONFIG_HPP_SNORTSAM

#include "Persistency/IPTypes.hpp"
#include "Persistency/PortNumber.hpp"
#include "Trigger/Simple/ThresholdConfig.hpp"
#include "Trigger/SnortSam/Who.hpp"
#include "Trigger/SnortSam/How.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief module's configuration representation.
 */
class Config: public Persistency::IPTypes<Config>
{
public:
  /** \brief create configration description.
   *  \param host     hostname to connect to.
   *  \param port     port to use.
   *  \param key      key for the encryption.
   *  \param who      who should be blocked.
   *  \param how      how host(s) should be blocked.
   *  \param duration length of block (in seconds).
   *  \param th       threshold configuration - informs when run trigger.
   */
  Config(const std::string             &host,
         const Persistency::PortNumber  port,
         const std::string             &key,
         const Who                      who,
         const How                      how,
         const unsigned int             duration,
         const Simple::ThresholdConfig &th):
    host_(host),
    port_(port),
    key_(key),
    who_(who),
    how_(how),
    duration_(duration),
    th_(th)
  {
  }

  /** \brief get host name/ip.
   */
  const std::string &getHost(void) const
  {
    return host_;
  }
  /** \brief get port on the agent (host).
   */
  Persistency::PortNumber getPort(void) const
  {
    return port_;
  }
  /** \brief gets secrete key, usd for cryptography.
   */
  const std::string &getKey(void) const
  {
    return key_;
  }
  /** \brief gets src/dst setting.
   */
  Who getWho(void) const
  {
    return who_;
  }
  /** \brief gets in/out (direction) setting.
   */
  How getHow(void) const
  {
    return how_;
  }
  /** \brief gets duration of given ban.
   */
  unsigned int getDuration(void) const
  {
    return duration_;
  }
  /** \brief get thresholds configuration.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }

private:
  std::string             host_;
  Persistency::PortNumber port_;
  std::string             key_;
  Who                     who_;
  How                     how_;
  unsigned int            duration_;
  Simple::ThresholdConfig th_;
}; // class Config

} // namespace SnortSam
} // namespace Trigger

#endif
