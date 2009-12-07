/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_TRANSACTIONAPI_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>


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
  /** \brief start virtual d-tors on the base class.
   */
  virtual ~TransactionAPI(void)
  {
  }
  /** \brief transaction acceptance opration's interface.
   */
  virtual void commit(void) = 0;
  /** \brief transaction abort interface.
   */
  virtual void rollback(void) = 0;
}; // class TransactionAPI

} // namespace IO
} // namespace Persistency

#endif
