/*
 * IDAssigner.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE
#define INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Logger/Node.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{

/** \brief implementation that uses DynamicConfig to communicate with persistent storage.
 */
struct IDAssigner: private boost::noncopyable
{
  /** \brief create object that will assign new IDs.
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   */
  IDAssigner(IO::ConnectionPtrNN conn, IO::Transaction &t);

  /** \brief get next ID value to be used
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   *  \return free ID value, that can be used for new MetaAlert.
   */
  MetaAlert::ID assign(IO::ConnectionPtrNN conn, IO::Transaction &t);

private:
  // TODO: add check if there is at most one instance of this object created
  //       at a given time.
  Base::Threads::Mutex   mutex_;
  Logger::Node           log_;
  MetaAlert::ID::Numeric nextFreeID_;
}; // struct IDAssigner

} // namespace Persistency

#endif
