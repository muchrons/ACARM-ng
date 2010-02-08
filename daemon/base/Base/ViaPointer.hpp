/*
 * ViaPointer.hpp
 *
 */
#ifndef INCLUDE_BASE_VIAPOINTER_HPP_FILE
#define INCLUDE_BASE_VIAPOINTER_HPP_FILE

/* public header */

#include <cstring>
#include <cassert>

namespace Base
{
/** \brief performs certain checks on pointers.
 */
struct ViaPointer
{
  /** \brief checks if values represented by pointers are equal.
   *  \param t1 first pointer.
   *  \param t2 second pointer.
   *  \return true if pointers pointo to identical values (but not necessary
   *          the same instance!) or are both NULLs, or false otherwise.
   */
  template<typename T>
  static bool equal(const T *t1, const T *t2);

private:
  template<typename T>
  static bool equalImpl(const T *t1, const T *t2)
  {
    // both NULLs or identical
    if(t1==t2)
      return true;

    // just one NULL
    if(t1==NULL || t2==NULL)
      return false;

    // comapre objects
    assert(t1!=NULL);
    assert(t2!=NULL);
    return *t1==*t2;
  }

  // no instances are needed.
  ViaPointer(void);
}; // struct ViaPointer


template<typename T>
inline bool ViaPointer::equal(const T *t1, const T *t2)
{
  return equalImpl(t1, t2);
}

template<>
inline bool ViaPointer::equal<char>(const char *t1, const char *t2)
{
  if( equalImpl(t1, t2)==false )
    return false;
  // if begining of the string matches, compare whole strings.
  return strcmp(t1, t2)==0;
}

} // namespace Base

#endif
