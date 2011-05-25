/*
 * UniqueTypeString.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_UNIQUETYPESTRING_HPP_FILE
#define INCLUDE_SYSTEM_UNIQUETYPESTRING_HPP_FILE

/* public header */

#include <algorithm>
#include <boost/operators.hpp>

#include "System/detail/Comparer.hpp"

namespace System
{

/** \brief helper for creating strings that have unique types.
 *
 *  common code base for simple string types, that need to be separate
 *  types, to prevent misusage (i.e. they should not be used interchangeably).
 *
 *  \param T unique class type (for distinguish names).
 *
 *  <code>
 *    namespace detail
 *    {
 *    struct MyStringType1
 *    {
 *      typedef std::string StringType;
 *    };
 *    } // namespace detail
 *
 *    typedef System::UniqueTypeString<detail::MyStringType1> MyStringType1;
 *  </code>
 */
template<typename T>
class UniqueTypeString: public boost::equality_comparable< UniqueTypeString<T> >,
                        public boost::less_than_comparable< UniqueTypeString<T> >,
                        public boost::equivalent< UniqueTypeString<T> >
{
public:
  /** \brief internal representation type. */
  typedef typename T::StringType value_type;

  /** \brief create object from C++ string.
   *  \param str string to be copied.
   */
  explicit UniqueTypeString(const value_type &str):
    str_(str)
  {
  }

  /** \brief gets string held inside.
   *  \return represented string.
   */
  const value_type &str(void) const
  {
    return str_;
  }

  /** \brief swap contents of two instances.
   *  \param other element to swap content with.
   */
  void swap(UniqueTypeString<T> &other)
  {
    std::swap(str_, other.str_);
  }

  /** \brief less-than comparison operator.
   *  \param other object to compare with.
   *  \return true, if this string is less than other string, false otherwise.
   */
  bool operator<(const UniqueTypeString<T> &other) const
  {
    return detail::Comparer<value_type>::lessThan( str(), other.str() );
  }

private:
  value_type str_;
}; // class UniqueTypeString

} // namespace System


namespace std
{
/** \brief swaps two elements of a System::UniqueTypeString<T> type.
 *  \param o1 object to swap.
 *  \param o2 object to swap with.
 */
template<typename T>
void swap(::System::UniqueTypeString<T> &o1, ::System::UniqueTypeString<T> &o2)
{
  o1.swap(o2);
} // swap()
} // namespace std

#endif
