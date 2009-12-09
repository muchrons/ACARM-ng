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
  void commit(void);
  /** \brief transaction abort interface.
   */
  void rollback(void);

protected:
  /** \brief open transaction.
   *  \param mutex mutex that data base connection is protected with.
   *  \param name  name of the transaction.
   */
  TransactionAPI(Base::Threads::Mutex &mutex, const std::string &name);

private:
  virtual void commitImpl(void) = 0;
  virtual void rollbackImpl(void) = 0;

  Base::Threads::Lock lock_;
  const std::string   name_;
}; // class TransactionAPI


typedef std::auto_ptr<TransactionAPI> TransactionAPIAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
