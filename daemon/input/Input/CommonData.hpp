/*
 * CommonData.hpp
 *
 */
#ifndef INCLUDE_INPUT_COMMONDATA_HPP_FILE
#define INCLUDE_INPUT_COMMONDATA_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Input/detail/IDMap.hpp"

namespace Input
{

/** \brief common data shared between inputs.
 */
class CommonData: private boost::noncopyable
{
public:
  /** \brief create common data; read required paramters from persistency.
   *  \param conn connetion to be used for reading.
   *  \param t    transaction to use for persistency I/O.
   *
   *  this object is to be used with different inputs at the same time
   *  and so does not keep given connection inside, but just uses it to
   *  read parameters and then releases back. the same gos for the transaction.
   */
  CommonData(Persistency::IO::ConnectionPtrNN  conn,
             Persistency::IO::Transaction     &t);

  /** \brief get common IDMap object's instance.
   *  \return object's reference.
   */
  detail::IDMap &getIDMap(void)
  {
    return idMap_;
  }

private:
  detail::IDMap idMap_;
}; // class CommonData


/** \brief non-NULL pointer to common inputs' data. */
typedef Commons::SharedPtrNotNULL<CommonData> CommonDataPtrNN;

} // namespace Input

#endif
