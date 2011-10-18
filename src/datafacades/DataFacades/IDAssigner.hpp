/*
 * IDAssigner.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_IDASSIGNER_HPP_FILE
#define INCLUDE_DATAFACADES_IDASSIGNER_HPP_FILE

/* public header */

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "System/Singleton.hpp"
#include "System/OneInstanceAtOnce.hpp"
#include "Base/Threads/Mutex.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace DataFacades
{
namespace detail
{

/** \brief wrapper for creating IDs.
 *
 *  \warning do NOT use this class directly. use singleton declared below instead!
 */
class IDAssigner: private System::OneInstanceAtOnce<IDAssigner>
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
  Persistency::MetaAlert::ID assignMetaAlertID(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);
  /** \brief get next ID value to be used
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   *  \return free ID value, that can be used for new Analyzer.
   */
  Persistency::Analyzer::ID assignAnalyzerID(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);

private:
  template<typename T>
  typename T::ID assign(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t);

  class IDAssignerPimpl;

  Base::Threads::Mutex               mutex_;
  boost::scoped_ptr<IDAssignerPimpl> pimpl_;
}; // class IDAssigner

} // namespace detail


/** \brief IDAssigner's singleton.
 */
typedef System::Singleton<detail::IDAssigner> IDAssigner;

} // namespace DataFacades

#endif
