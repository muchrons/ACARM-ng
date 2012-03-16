/*
 * NullValue.hpp
 *
 */
#ifndef INCLUDE_BASE_NULLVALUE_HPP_FILE
#define INCLUDE_BASE_NULLVALUE_HPP_FILE

/* public header */

#include <cstdlib>
#include <boost/operators.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

#include "Base/ViaPointer.hpp"

namespace Base
{
/** \brief class holding simple value and returning pointer to it or NULL.
 */
template<typename T>
class NullValue: public boost::equality_comparable< NullValue<T> >
{
public:
  /** \brief create class from pointer.
   *  \param value pointer to create from.
   */
  explicit NullValue(const T *value=NULL):
    isNull_(value==NULL),
    value_( IfNeeded<T, boost::is_arithmetic<T>::value>::getDefault(value) )
  {
  }
  /** \brief create class from copy.
   *  \param value value to represent
   */
  explicit NullValue(const T &value):
    isNull_(false),
    value_(value)
  {
  }
  /** \brief get pointer value (may be NULL).
   */
  const T *get(void) const
  {
    if(isNull_)
      return NULL;
    return &value_;
  }
  /** \brief gives pointer-like access to object held inside.
   */
  const T *operator->(void) const
  {
    assert( get()!=NULL );
    return get();
  }
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const NullValue<T> &other) const
  {
    return Base::ViaPointer::equal( get(), other.get() );
  }

private:
  template<typename V, bool IsNumber>
  struct IfNeeded           // number
  {
    /** \brief automatic zeroing numbers
     *  \param v value.
     *  \return 0 if value is NULL, value othervise.
     */
    static V getDefault(const V *v)
    {
      if(v==NULL)
        return 0;
      return *v;
    }
  }; // struct IfNeeded
  template<typename V>
  struct IfNeeded<V, false> // non-number
  {
    /** \brief automatic zeroing numbers
     *  \param v value.
     *  \return 0 if value is NULL, value othervise.
     */
    static V getDefault(const V *v)
    {
      if(v==NULL)
        return V();
      return *v;
    }
  }; // struct IfNeeded

  bool isNull_;
  T    value_;
}; // class NullValue

} // namespace Base

#endif
