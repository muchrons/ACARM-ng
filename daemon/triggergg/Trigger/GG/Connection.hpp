/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_CONNECTION_HPP_FILE
#define INCLUDE_TRIGGER_GG_CONNECTION_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libgadu.h>
#include <cassert>

#include "Base/Threads/Mutex.hpp"
#include "Logger/Node.hpp"
#include "Commons/Threads/Thread.hpp"
#include "Trigger/GG/LoginParameters.hpp"
#include "Trigger/GG/AccountConfig.hpp"
#include "Trigger/GG/AutoSession.hpp"
#include "Trigger/GG/ExceptionConnectionError.hpp"
#include "Trigger/GG/ExceptionSendingError.hpp"

namespace Trigger
{
namespace GG
{

/** \brief class representing (estabilished) connection to GG server.
 */
class Connection: private boost::noncopyable
{
public:
  /** \brief as long as this class exists no pings will be send in a
   *         background process. this is workaround for thread synchronization.
   */
  class HoldPings: private boost::noncopyable
  {
  public:
    /** \brief stop sending pings.
     *  \param conn connection to block pings for.
     *  \note if ping I/O is on the way call will block until it's done.
     */
    explicit HoldPings(Connection &conn):
      conn_(conn)
    {
      conn_.stopPings();
    }
    /** \brief enable pings back.
     */
    ~HoldPings(void)
    {
      conn_.startPings();
    }

  private:
    Connection &conn_;
  }; // class HoldPings

  /** \brief connect to GG-server with given acount.
   *  \param cfg account's configuration to use when connecting.
   */
  explicit Connection(const AccountConfig &cfg);
  /** \brief disconnect from server.
   */
  ~Connection(void);

  /** \brief get session structure - const version.
   *  \return session pointer.
   *  \note pointer is guaranteed to be non-NULL.
   */
  const gg_session *get(const HoldPings &) const
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }
  /** \brief get session structure - non-const version.
   *  \return session pointer.
   *  \note pointer is guaranteed to be non-NULL.
   */
  gg_session *get(const HoldPings &)
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }

  /** \brief send ping to server.
   *  \note request will be ignored if stopPings() has been called.
   */
  void ping(void);

private:
  friend class HoldPings;
  void stopPings(void);
  void startPings(void);

  gg_session *connect(void) const;
  void sendContactsList(gg_session *sess);

  Logger::Node                 log_;
  const LoginParameters        params_;
  AutoSession                  sess_;
  unsigned int                 pingBlocksCount_;
  Commons::Threads::Thread     pingThread_;
  mutable Base::Threads::Mutex mutex_;
}; // class Connection

} // namespace GG
} // namespace Trigger

#endif
