/*
 * Heartbeats.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_HEARTBEATS_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_HEARTBEATS_HPP_FILE

/* public header */

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

#include "Commons/LimitedString.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Exception.hpp"

namespace Persistency
{
namespace IO
{

/** \brief heartbeats handling interface.
 *
 *  each subsystem of ACARM-ng, that report heartbeats can send it
 *  for itself as well as for other modules it works with, so that
 *  full information will be available in one place.
 *
 *  example of such a situation is a plugin keeping connection with a foregin
 *  system, wanting to report if it is online or not.
 */
class Heartbeats: private boost::noncopyable
{
public:
  /** \brief type representing owner's name. */
  typedef Commons::LimitedString<64>  Owner;
  /** \brief type representing module's name. */
  typedef Commons::LimitedString<128> Module;

  /** \brief create heartbeats generator.
   *  \param owner system that will report heartbeats (i.e. ACARM-ng element).
   *  \param t     active transaction.
   */
  Heartbeats(const Owner &owner, Transaction &t);
  /** \brief virtual d-tor for polymorphic base class.
   */
  virtual ~Heartbeats(void);

  /** \brief send heartbeat to the system.
   *  \param m       module that heartbeat can report for.
   *  \param timeout number of seconds this entry is valid. if no heartbeat arrives within
   *                 this time module is considered dead.
   */
  void report(const Module &m, unsigned int timeout);

private:
  virtual void reportImpl(Transaction &t, const Owner &o, const Module &m, Timestamp reported, unsigned int timeout) = 0;

  const Owner  owner_;
  Transaction &t_;
}; // class Heartbeats


/** \brief auto pointer to IO::Heartbeats class. */
typedef std::auto_ptr<Heartbeats> HeartbeatsAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
