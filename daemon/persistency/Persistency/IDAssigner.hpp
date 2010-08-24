/*
 * IDAssigner.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE
#define INCLUDE_PERSISTENCY_IDASSIGNER_HPP_FILE

/* public header */

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "System/Singleton.hpp"
#include "Base/Threads/Mutex.hpp"
#include "Logger/Node.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{
namespace detail
{
// forward declaration for friend declaration
class IDAssignerWrapper;

/** \brief implementation that uses DynamicConfig to communicate with persistent storage.
 */
class IDAssigner: private boost::noncopyable
{
public:
  /** \brief get next ID value to be used
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   *  \return free ID value, that can be used for new MetaAlert.
   */
  MetaAlert::ID assign(IO::ConnectionPtrNN conn, IO::Transaction &t);

private:
  // ensure only wrapper can make instance
  friend class IDAssignerWrapper;
  IDAssigner(IO::ConnectionPtrNN conn, IO::Transaction &t);

  // TODO: add check if there is at most one instance of this object created
  //       at a given time.
  Base::Threads::Mutex   mutex_;
  Logger::Node           log_;
  MetaAlert::ID::Numeric nextFreeID_;
}; // class IDAssigner

/** \brief wrapper making deafult c-tor.
 */
class IDAssignerWrapper: private boost::noncopyable
{
public:
  /** \brief get next ID value to be used
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   *  \return free ID value, that can be used for new MetaAlert.
   */
  MetaAlert::ID assign(IO::ConnectionPtrNN conn, IO::Transaction &t);

private:
  // ensure only singleton makes new instances
  friend class System::Singleton<IDAssignerWrapper>;
  IDAssignerWrapper(void)
  {
  }

  Base::Threads::Mutex          mutex_;
  boost::scoped_ptr<IDAssigner> impl_;
}; // class IDAssignerWrapper

} // namespace detail


/** \brief IDAssigner's singleton.
 */
typedef System::Singleton<detail::IDAssignerWrapper> IDAssigner;

} // namespace Persistency

#endif
