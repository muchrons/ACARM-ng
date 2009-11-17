/*
 * NullValue.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_DETAIL_NULLVALUE_HPP_FILE
#define INCLUDE_PERSISTENCY_DETAIL_NULLVALUE_HPP_FILE

/* public header */

#include <cstdlib>


namespace Persistency
{
namespace detail
{
/** \brief class holding simple value and returning pointer to it or NULL>
 */
template<typename T>
class NullValue
{
public:
  /** \brief create class from pointer.
   *  \param value pointer to create from.
   */
  explicit NullValue(const T *value=NULL):
    isNull_(value==NULL)
  {
    if(!isNull_)
      value_=*value;
  }
  /** \brief get pointer value (may be NULL).
   */
  const T *get(void) const
  {
    if(isNull_)
      return NULL;
    return &value_;
  }

private:
  bool isNull_;
  T    value_;
}; // class NullValue

} // namespace detail
} // namespace Persistency

#endif
