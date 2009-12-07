/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_TRANSACTIONAPI_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"


namespace Persistency
{
namespace IO
{

/** \brief transaction calls that are persistency-type-dependent.
 *  \note transaction must begin in derived object's c-tor.
 */
class TransactionAPI: private boost::noncopyable
{
public:
  /** \brief unlocks connection's mutex.
   */
  virtual ~TransactionAPI(void);

  /** \brief transaction acceptance opration's interface.
   */
  virtual void commit(void) = 0;
  /** \brief transaction abort interface.
   */
  virtual void rollback(void) = 0;

protected:
  /** \brief open transaction.
   *  \param mutex mutex that data base connection is protected with.
   */
  explicit TransactionAPI(Base::Threads::Mutex &mutex);

private:
  Base::Threads::Lock lock_;
}; // class TransactionAPI


typedef std::auto_ptr<TransactionAPI> TransactionAPIAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
