/*
 * GlobalConnection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_GLOBALCONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_GLOBALCONNECTION_HPP_FILE

#include <string>

#include "System/Singleton.hpp"
#include "Base/Threads/Mutex.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Exception.hpp"


namespace Persistency
{
namespace IO
{

/** \brief class representing globaly accessable persistency-conneciton.
 *
 *  this solution provides thread-safe connection to persistency for small
 *  operations in the system, that are not normally associated with any
 *  particular thread, or are not known a priory when/where will they be executer
 *  thus they cannot be assigned to thread's conneciton explicitly.
 *
 *  example of such operations are destructors of shared objects.
 *
 *  note that all of calls to methods of this class are synchronized and cannot
 *  be executed as a part of external transaction - they provide single,
 *  per-call transaction that is commited stright away. this does not make it
 *  very efficient, when calling often from multiple threads, but it is
 *  thread-safe this way and does not require esabilishing new connecitons
 *  each time small, rare opration has to be performed.
 */
class GlobalConnectionImpl: private boost::noncopyable
{
public:
  /** \brief marks given element as unused.
   *  \param ma meta alert to be marked as unused.
   */
  void markAsUnused(MetaAlertPtrNN ma);

private:
  friend class System::Singleton<GlobalConnectionImpl>;
  GlobalConnectionImpl(void);

  Base::Threads::Mutex mutex_;
  ConnectionPtrNN      conn_;
}; // class GlobalConnectionImpl


/** \brief singleton allowing access to GlobalConnectionImpl.
 *  \note this is for Persistency's internal usage only!
 */
typedef System::Singleton<GlobalConnectionImpl> GlobalConnection;

} // namespace IO
} // namespace Persistency

#endif
