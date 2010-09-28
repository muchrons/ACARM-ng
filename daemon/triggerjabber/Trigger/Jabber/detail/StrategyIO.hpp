/*
 * StrategyIO.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_DETAIL_STRATEGYIO_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_DETAIL_STRATEGYIO_HPP_FILE

#include <memory>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Logger/Node.hpp"
#include "Trigger/Jabber/Config.hpp"
#include "Trigger/Jabber/Connection.hpp"

namespace Trigger
{
namespace Jabber
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

  /** \brief discard all messages that could have arrived.
   */
  void discardIncommingMessages(void);

private:
  void reconnectIfNeeded(void);

  mutable Base::Threads::Mutex mutex_;      ///< mutex for thread-safety
  const Logger::Node           log_;        ///< used for logging
  const AccountConfig          jabberCfg_;  ///< account's config
  const std::string            receiver_;   ///< UID or message receiver
  ConnectionAutoPtr            conn_;       ///< connection to be used
}; // struct StrategyIO

} // namespace detail
} // namespace Jabber
} // namespace Trigger

#endif
