/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_TRANSACTIONAPI_HPP_FILE

/* public header */

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"


namespace Persistency
{
namespace IO
{

namespace detail
{
// forward declaration.
class TransactionAPIOperations;
} // namespace detail


/** \brief transaction calls that are persistency-type-dependent.
 *  \note transaction must begin in derived object's c-tor.
 */
class TransactionAPI: private boost::noncopyable
{
public:
  /** \brief unlocks connection's mutex.
   */
  virtual ~TransactionAPI(void);
  /** \brief gets transaciton name.
   *  \return name of this transaction.
   */
  const std::string getName(void) const;

protected:
  /** \brief open transaction.
   *  \param mutex mutex that data base connection is protected with.
   *  \param name  name of the transaction.
   */
  TransactionAPI(Base::Threads::Mutex &mutex, const std::string &name);

private:
  /** \brief transaction acceptance opration's interface.
   */
  void commit(void);
  /** \brief transaction abort interface.
   */
  void rollback(void);

  virtual void commitImpl(void) = 0;
  virtual void rollbackImpl(void) = 0;

  friend class detail::TransactionAPIOperations;

  Base::Threads::Lock lock_;
  const std::string   name_;
  Logger::Node        log_;
}; // class TransactionAPI


/** \brief auto pointer to IO::TransactionAPI class. */
typedef std::auto_ptr<TransactionAPI> TransactionAPIAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
