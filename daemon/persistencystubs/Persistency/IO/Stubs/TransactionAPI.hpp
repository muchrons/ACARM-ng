/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_TRANSACTIONAPI_HPP_FILE

#include "Persistency/IO/TransactionAPI.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief transaction calls that are persistency-type-dependent.
 *  \note transaction must begin in derived object's c-tor.
 */
class TransactionAPI: public IO::TransactionAPI
{
public:
  /** \brief open transaction.
   *  \param mutex mutex that data base connection is protected with.
   */
  TransactionAPI(Base::Threads::Mutex &mutex, const std::string &name);

private:
  virtual void commitImpl(void);
  virtual void rollbackImpl(void);
}; // class TransactionAPI

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
