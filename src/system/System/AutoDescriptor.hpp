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
#include <boost/operators.hpp>

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
    _v(v)
  {
  }
  /** \brief returns value of descriptor.
   *  \return descriptor being held.
   */
  inline TValue get(void) const
  {
    return _v;
  }
  /** \brief closes descriptor held inside.
   */
  void deallocate(void)
  {
    if(_v!=-1)
    {
      close(_v);
      _v=-1;
    }
  }

private:
  TValue _v;
}; // class DescriptorHolder

} // namespace detail

/** \brief typedef for descriptor hoder.
 */
typedef System::AutoVariable<detail::DescriptorHolder> AutoDescriptor;

} // namespace System

#endif
