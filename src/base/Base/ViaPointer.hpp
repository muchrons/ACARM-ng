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
   *  \return true if pointers points to identical values (but not necessary
   *          the same instance!) or are both NULLs; false otherwise.
   */
  template<typename T>
  static bool equal(const T *t1, const T *t2);

  /** \brief checks if values represented by pointers are less-than.
   *  \param t1 first pointer.
   *  \param t2 second pointer.
   *  \return true if pointer t1 points to less-than t2 value (but not necessary
   *          the same instance!), false otherwise.
   *  \note NULL is always threated as the 'lesser' value.
   */
  template<typename T>
  static bool lessThan(const T *t1, const T *t2);

private:
  // no instances are needed.
  ViaPointer(void);
}; // struct ViaPointer


template<typename T>
inline bool ViaPointer::equal(const T *t1, const T *t2)
{
  // both NULLs are identical
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

/** \brief checks if values represented by C-string pointers are equal.
 *  \param t1 first pointer.
 *  \param t2 second pointer.
 *  \return true if pointers repreesnt identical C-strings (but not necessary
 *          the same instance!) or are both NULLs; false otherwise.
 */
template<>
inline bool ViaPointer::equal<char>(const char *t1, const char *t2)
{
  // both NULLs are identical
  if(t1==t2)
    return true;

  // just one NULL
  if(t1==NULL || t2==NULL)
    return false;

  // compare whole strings.
  assert(t1!=NULL);
  assert(t2!=NULL);
  return strcmp(t1, t2)==0;
}


template<typename T>
inline bool ViaPointer::lessThan(const T *t1, const T *t2)
{
  if(t1==NULL)
    return true;
  if(t2==NULL)
    return t1==NULL;    // if t1 is not NULL, order has to be switched

  // comapre objects
  assert(t1!=NULL);
  assert(t2!=NULL);
  return *t1<*t2;
}

/** \brief checks if values represented by pointers are less-than.
 *  \param t1 first pointer.
 *  \param t2 second pointer.
 *  \return true if pointer t1 points to less-than t2 value (but not necessary
 *          the same instance!), false otherwise.
 *  \note NULL is always threated as the 'lesser' value.
 */
template<>
inline bool ViaPointer::lessThan<char>(const char *t1, const char *t2)
{
  if(t1==NULL)
    return true;
  if(t2==NULL)
    return t1==NULL;    // if t1 is not NULL, order has to be switched

  // compare whole strings.
  assert(t1!=NULL);
  assert(t2!=NULL);
  return strcmp(t1, t2)<0;
}

} // namespace Base

#endif
