/*
 * checkEquality.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_CHECKEQUALITY_T_HPP_FILE
#define INCLUDE_PERSISTENCY_CHECKEQUALITY_T_HPP_FILE

#include <tut/tut.hpp>

namespace Persistency
{

/** \brief helper to check == and != operators in given type.
 *  \param t1  first test object
 *  \param t1o second test object, that is equal to t1, but is a second
 *             instance (finds places where only pointer is compared instead
 *             of actuall value).
 *  \param t2  other object, that differs from t1 (and so t1o).
 *
 *  \note t1 and t1o must differ from t2!
 */
template<typename T>
void checkEquality(const T &t1, const T &t1o, const T &t2)
{
  // test equality
  tut::ensure("comparing identical elements with == failed",          t1==t1o  );
  // test if equality fails for different eleemnts
  tut::ensure("comparing different elements with == didn't failed", !(t1==t2)  );

  // test inequality
  tut::ensure("comparing identical elements with != didn't failed", !(t1!=t1o) );
  // test if equality fails for different eleemnts
  tut::ensure("comparing different elements with != failed",          t1!=t2   );
} // checkEqualit()

/** \brief helper to check == and != operators in given type.
 *  \param t1  first test object
 *  \param t2  other object, that differs from t1.
 *
 *  \note t1 and t1o must differ from t2!
 */
template<typename T>
void checkEquality(const T &t1, const T &t2)
{
  checkEquality(t1, t1, t2);
} // checkEquality()

} // namespace Persistency

#endif
