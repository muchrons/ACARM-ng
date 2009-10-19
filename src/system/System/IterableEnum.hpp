/*
 * IterableEnum.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_ITERABLEENUM_HPP_FILE
#define INCLUDE_SYSTEM_ITERABLEENUM_HPP_FILE

/* public header */

#include <boost/operators.hpp>
#include <boost/static_assert.hpp>
#include <cassert>

#include "System/Enum.hpp"


namespace System
{

/** \brief Class wrapping around user-specified enum that
 *         has continous numering. it can be enumerable then.
 *
 *  reference usage:
 *
 *  \code
 *
 *   struct MyEnumToWrap
 *   {
 *     // note that values have to be continues and increasing
 *     // from Min to Max!
 *     typedef enum
 *     {
 *       A=3,
 *       B=4,
 *       C=5
 *     } Type;
 *     enum { Min=A };
 *     enum { Max=C };
 *   };
 *
 *   // [...]
 *
 *   typedef System::IterableEnum<MyEnumToWrap> IterableEnum1;
 *
 *   // [...]
 *
 *   IterableEnum1 e;
 *
 *  \endcode
 *
 */
template<typename E>
class IterableEnum: public Enum<E>
{
public:
  /** \brief ThisType typedef.  */
  typedef IterableEnum<E>       TType;
  /** \brief BaseType typedef.  */
  typedef Enum<E>               BType;
  /** \brief EnumType typedef.  */
  typedef typename BType::EType EType;

  /** \brief const iterator definition.  */
  class const_iterator: public boost::equality_comparable<const_iterator>
  {
  public:
    /** \brief create object.
     */
    const_iterator(void):
      e_( static_cast<EType>( static_cast<int>(E::Min) ) )
    {
      check(e_);
    }
    /** \brief create iterator from given element.
     *  \param e element to start iteration from.
     */
    explicit const_iterator(const EType e):
      e_(e)
    {
      check(e_);
    }
    /** \brief dereference operator.
     *  \return current element.
     */
    EType operator*(void) const
    {
      check(e_);
      return e_;
    }
    /** \brief compare iterators.
     *  \param it iterator to compare with.
     *  \return true if iterators are equal, false otherwise.
     */
    bool operator==(const const_iterator &it) const
    {
      check(e_);
      check(it.e_);
      return e_==it.e_;
    }

    /** \brief pre-incrementation.
     *  \return reference to current object.
     */
    const const_iterator &operator++(void)
    {
      check(e_);
      e_=static_cast<EType>(e_+1);
      check(e_);
      return *this;
    }
    /** \brief post-incrementation.
     *  \return reference to previous value of itertor.
     */
    const_iterator operator++(int)
    {
      check(e_);
      const const_iterator tmp=*this;
      e_=static_cast<EType>(e_+1);
      check(e_);
      return tmp;
    }

  private:
    inline void check(const EType e) const
    {
      // following asserts are a bit strange formulas, but they disables
      // warning on some newer compilers, saying that comparison is always
      // true due to limited range of type. the idea is to transform
      // abvious "a<=b" to "a-b<=0".

      // check minimum value
      assert( static_cast<int>(E::Min)-static_cast<int>(e)         <=0 );
      // Max+1 since const_iterator may represent end()
      // iterator as well.
      assert( static_cast<int>(e)     -(static_cast<int>(E::Max)+1)<=0 );

      // this supress warning in release mode
      ignoreParam(e);
    }
    EType e_;
  }; // class const_iterator

  /** \brief gives compile-time size of container.  */
  enum { Size=E::Max-E::Min+1 };

  /** \brief construction from enum.
   *  \param t enum to be constructed from.
   */
  template<typename T>
  inline IterableEnum(const T t):   // 'explicit' is not welcomed here
    BType(t)
  {
    check(t);
  }

  /** \brief  returns iterator to enum's begin
   *  \return begin iterator.
   */
  static inline const_iterator begin(void)
  {
    const EType val=static_cast<EType>( static_cast<int>(E::Min)+0 );
    return const_iterator(val);
  }

  /** \brief  returns iterator to enum's end
   *  \return end iterator.
   */
  static inline const_iterator end(void)
  {
    const EType val=static_cast<EType>( static_cast<int>(E::Max)+1 );
    return const_iterator(val);
  }

  /** \brief  computes numer of values in enum.
   *  \return number of elements between Min and Max.
   */
  static inline size_t size(void)
  {
    return Size;
  }

private:
  // ensure that Min value is not greater than Max value.
  // cast here suppress warning when E is decalred in
  // unnamed namespace.
  BOOST_STATIC_ASSERT( static_cast<int>(E::Min)<=
                       static_cast<int>(E::Max)   );

  template<typename T>
  inline void check(const T t) const
  {
    check( t.toInt() );
  }
  inline void check(const EType e) const
  {
    check( static_cast<int>(e) );
  }
  inline void check(const int l) const
  {
    assert( E::Min<=l );
    assert( l<=E::Max );
    // this supresses warning in release mode
    ignoreParam(l);
  }
  // this call supresses warrning from compiler abiut unused
  // variable from argument list, in release mode.
  static inline void ignoreParam(const long)
  {
  }
}; // class IterableEnum

} // namespace System

#endif

