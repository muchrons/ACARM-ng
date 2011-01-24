/*
 * Default.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_DEFAULT_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_DEFAULT_HPP_FILE

#include "Base/NullValue.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

/** \brief helper object to create NULL objects.
 *
 *  this is helper code for ReaderHelper<> template.
 */
template<typename T>
struct Default
{
  /** \brief create NULL using c-tor with pointer.
   *  \return object instance.
   */
  static inline T null(void)
  {
    return T(NULL);
  }
}; // struct Default


/** \brief specialization to create empty NullValue<> instance.
 */
template<typename T>
struct Default< Base::NullValue<T> >
{
  /** \brief create NULL using default c-tor.
   *  \return object instance.
   */
  static inline Base::NullValue<T> null(void)
  {
    return Base::NullValue<T>();
  }
}; // struct Default

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
