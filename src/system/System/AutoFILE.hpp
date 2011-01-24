/*
 * AutoFILE.hpp
 *
 * class behaving like an auto_ptr<> for
 * FILE streams.
 *
 */
#ifndef INCLUDE_SYSTEM_AUTOFILE_HPP_FILE
#define INCLUDE_SYSTEM_AUTOFILE_HPP_FILE

/* public header */

#include <stdio.h>
#include <unistd.h>
#include <boost/operators.hpp>

#include "System/AutoVariable.hpp"

namespace System
{
namespace detail
{

/** \brief helper class for internal usage in AutoFILE.
 */
class FILEHolder
{
public:
  /** \brief typedef for value type being held.
   */
  typedef FILE* TValue;
  /** \brief constructor setting start FILE pointer
   *         to be held.
   *  \param v FILE to be held.
   */
  explicit FILEHolder(TValue v=NULL):
    v_(v)
  {
  }
  /** \brief returns value of descriptor.
   *  \return descriptor being held.
   */
  inline TValue get(void) const
  {
    return v_;
  }
  /** \brief closes descriptor held inside.
   */
  void deallocate(void)
  {
    if(v_!=NULL)
    {
      fclose(v_);
      v_=NULL;
    }
  }

private:
  TValue v_;
}; // class FILEHolder

} // namespace detail

/** \brief declaration of auto FILE holder.
 */
typedef System::AutoVariable<detail::FILEHolder> AutoFILE;

} // namespace System

#endif
