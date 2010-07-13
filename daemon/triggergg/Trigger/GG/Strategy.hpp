/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE

#include <memory>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/GG/Config.hpp"
#include "Trigger/GG/Connection.hpp"

namespace Trigger
{
namespace GG
{
namespace detail
{
/** \brief helper object holding connection.
 */
struct StrategyIO: private boost::noncopyable
{
  /** \brief connection auto pointer. */
  typedef std::auto_ptr<Connection> ConnectionAutoPtr;
  /** \brief create object.
   */
  StrategyIO(void):
    log_("trigger.gg.detail.strategyio")
  {
  }

  /** \brief ping gg server.
   */
  void ping(void);

  mutable Base::Threads::Mutex      mutex_; ///< mutex for thread-safety
  const Logger::Node                log_;   ///< used for logging
  ConnectionAutoPtr                 conn_;  ///< connection to be used
}; // struct StrategyIO
} // namespace detail

/** \brief Gadu-Gadu triggering strategy
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  /** \brief create strategy instance.
   *  \param cfg configuration to be applied.
   */
  explicit Strategy(const Config &cfg);

private:
  virtual void triggerImpl(const Node &n);

  const AccountConfig      ggCfg_;
  const UserID             receiver_;
  detail::StrategyIO       io_;
  Commons::Threads::Thread pingThread_;
}; // class Strategy

} // namespace GG
} // namespace Trigger

#endif
