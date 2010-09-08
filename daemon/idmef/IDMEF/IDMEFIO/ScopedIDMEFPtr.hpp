/*
 * ScopedIDMEFPtr.hpp
 *
 */
#ifndef INCLUDE_RFCIO_IDMEFIO_SCOPEDIDMEFPTR_HPP_FILE
#define INCLUDE_RFCIO_IDMEFIO_SCOPEDIDMEFPTR_HPP_FILE

#include "System/ScopedPtrCustom.hpp"
#include "RFCIO/IDMEFIO/libidmefHeaders.hpp"

namespace RFCIO
{
namespace IDMEFIO
{
namespace detail
{
/** \brief wrapper normalizing interface freeIDMEFPtrWrapper().
 *  \param ptr pointer to be freed.
 */
void freeIDMEFPtrWrapper(IDMEFPtr ptr)
{
  const int recursive=1;
  libidmef_idmefp_destroy(ptr, recursive);
} // freeIDMEFPtrWrapper()
} // namespace detail


/** \brief scoped pointer to IDMEF data type from C library. */
typedef System::ScopedPtrCustom<IDMEF, detail::freeIDMEFPtrWrapper> ScopedIDMEFPtr;

} // namespace IDMEFIO
} // namespace RFCIO

#endif
