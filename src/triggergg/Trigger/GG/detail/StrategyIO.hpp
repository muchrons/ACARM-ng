/*
 * StrategyIO.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_DETAIL_STRATEGYIO_HPP_FILE
#define INCLUDE_TRIGGER_GG_DETAIL_STRATEGYIO_HPP_FILE

#include <memory>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Logger/Node.hpp"
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
   *  \param cfg configuration to be used by trigger.
   */
  explicit StrategyIO(const Config &cfg);

  /** \brief send message.
   *  \param msg mesage to be send.
   */
  void send(const std::string &msg);

  /** \brief ping gg server.
   */
  void ping(void);

  /** \brief discard all messages that could have arrived.
   */
  void discardIncommingMessages(void);

private:
  void reconnectIfNeeded(void);

  mutable Base::Threads::Mutex mutex_;      ///< mutex for thread-safety
  const Logger::Node           log_;        ///< used for logging
  const AccountConfig          ggCfg_;      ///< account's config
  const Config::Receivers      receivers_;  ///< UID or message receiver
  ConnectionAutoPtr            conn_;       ///< connection to be used
}; // struct StrategyIO

} // namespace detail
} // namespace GG
} // namespace Trigger

#endif
