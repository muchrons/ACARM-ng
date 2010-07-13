/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE
#define INCLUDE_TRIGGER_GG_STRATEGY_HPP_FILE

#include <memory>

#include "Base/Threads/Mutex.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Trigger/Simple/Strategy.hpp"
#include "Trigger/GG/Config.hpp"
#include "Trigger/GG/Connection.hpp"

namespace Trigger
{
namespace GG
{

/** \brief Gadu-Gadu triggering strategy
 */
class Strategy: public Trigger::Simple::Strategy
{
public:
  /** \brief create strategy instance.
   *  \param cfg configuration to be applied.
   */
  explicit Strategy(const Config &cfg);

  /** \brief ping gg server.
   */
  void ping(void);

private:
  virtual void triggerImpl(const Node &n);

  typedef std::auto_ptr<Connection> ConnectionAutoPtr;

  const AccountConfig          ggCfg_;
  const UserID                 receiver_;
  ConnectionAutoPtr            conn_;
  mutable Base::Threads::Mutex mutex_;
  Commons::Threads::Thread     pingThread_; // keep this element as a last one!
}; // class Strategy

} // namespace GG
} // namespace Trigger

#endif
