/*
 * NoInstance.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_NOINSTANCE_HPP_FILE
#define INCLUDE_SYSTEM_NOINSTANCE_HPP_FILE

/* public header */

namespace System
{

/** \brief class ensuring no isntances will be mode.
 *
 *  this is usefull as a base class for those who do not want to
 *  have class instances. it should be privatly derived from,just
 *  like from boost:noncoyable.
 */
class NoInstance
{
private:
  // block creating instances of this class
  NoInstance(void);
}; // class NoInstance

} // namespace System

#endif

