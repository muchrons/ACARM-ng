/*
 * Enum.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_ENUM_HPP_FILE
#define INCLUDE_SYSTEM_ENUM_HPP_FILE

/* public header */

#include <boost/operators.hpp>
#include <boost/static_assert.hpp>
#include <cassert>

namespace System
{

/** \brief Class wrapping around user-specified enum.
 *
 *  reference usage:
 *
 *  \code
 *
 *   struct MyEnumToWrap
 *   {
 *     typedef enum
 *     {
 *       A,
 *       B,
 *       C
 *     } Type;
 *   };
 *
 *   // [...]
 *
 *   typedef System::Enum<MyEnumToWrap> Enum1;
 *
 *   // [...]
 *
 *   Enum1 e;
 *
 *  \endcode
 *
 */
template<typename E>
class Enum: public E,
            public boost::equality_comparable< Enum<E> >
{
public:
  /** \brief ThisType typedef.
   */
  typedef Enum<E>          TType;
  /** \brief EnumType typedef.
   */
  typedef typename E::Type EType;

  /** \brief construction from enum.
   *  \param e enum to be constructed from.
   */
  inline Enum(const EType e):   // 'explicit' is not welcomed here
    e_(e)
  {
  }

  /** \brief assignment from pure enum.
   *  \param e enum value to assign.
   *  \return this refference.
   */
  inline const TType& operator=(const EType e)
  {
    e_=e;
    return *this;
  }

  /** \brief comparison of two classes.
   *  \param t enum to comapre with.
   *  \return true if enums are equal.
   */
  inline bool operator==(const TType t) const
  {
    return e_==t.e_;
  }
  /** \brief compare enum class with enum value.
   *  \param e enum value to campare with.
   *  \return true is values are the same.
   */
  inline bool operator==(const EType e) const
  {
    return e_==e;
  }

  /** \brief backward compatibility call - do NOT use it anymore.
   *  \return enum value as integer.
   */
  inline long toLong(void) const
  {
    BOOST_STATIC_ASSERT( sizeof(e_)<=sizeof(long) );
    return e_;
  }
  /** \brief returns enum as a number.
   *  \return enum value as integer.
   */
  inline int toInt(void) const
  {
    BOOST_STATIC_ASSERT( sizeof(e_)==sizeof(int) );
    return e_;
  }

private:
  EType e_;
}; // class Enum

} // namespace System

#endif

