/*
 * IDAssigner.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_IDASSIGNER_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_IDASSIGNER_HPP_FILE

/* public header */

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "System/Singleton.hpp"
#include "Base/Threads/Mutex.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace Persistency
{
namespace Facades
{
namespace detail
{

/** \brief wrapper for creating IDs.
 */
class IDAssigner: private boost::noncopyable
{
public:
  /** \brief initialize object.
   */
  IDAssigner(void);
  /** \brief deallocate all object's resources.
   */
  ~IDAssigner(void);

  /** \brief get next ID value to be used
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   *  \return free ID value, that can be used for new MetaAlert.
   */
  MetaAlert::ID assignMetaAlertID(IO::ConnectionPtrNN conn, IO::Transaction &t);
  /** \brief get next ID value to be used
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   *  \return free ID value, that can be used for new Analyzer.
   */
  Analyzer::ID assignAnalyzerID(IO::ConnectionPtrNN conn, IO::Transaction &t);

private:
  template<typename T>
  typename T::ID assign(IO::ConnectionPtrNN conn, IO::Transaction &t);

  class IDAssignerPimpl;

  Base::Threads::Mutex               mutex_;
  boost::scoped_ptr<IDAssignerPimpl> pimpl_;
}; // class IDAssigner

} // namespace detail


/** \brief IDAssigner's singleton.
 */
typedef System::Singleton<detail::IDAssigner> IDAssigner;

} // namespace Facades
} // namespace Persistency

#endif
