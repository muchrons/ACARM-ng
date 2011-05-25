/*
 * AutoDescriptor.hpp
 *
 * class behaving like an auto_ptr<> for
 * file descriptors.
 *
 */
#ifndef INCLUDE_SYSTEM_AUTODESCRIPTOR_HPP_FILE
#define INCLUDE_SYSTEM_AUTODESCRIPTOR_HPP_FILE

/* public header */

#include <unistd.h>

#include "System/AutoVariable.hpp"

namespace System
{

namespace detail
{
/** \brief helper class for internal usage in AutoDescriptor.
 */
class DescriptorHolder
{
public:
  /** \brief typedef for value used as type being held.
   */
  typedef int TValue;
  /** \brief constructor setting start descriptor
   *         to be held.
   *  \param v descriptor to be held.
   */
  explicit DescriptorHolder(TValue v=-1):
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
    if(v_!=-1)
    {
      close(v_);
      v_=-1;
    }
  }

private:
  TValue v_;
}; // class DescriptorHolder

} // namespace detail

/** \brief typedef for descriptor hoder.
 */
typedef System::AutoVariable<detail::DescriptorHolder> AutoDescriptor;

} // namespace System

#endif
