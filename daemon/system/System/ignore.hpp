/*
 * ignore.hpp
 *
 * helper template to prevent warning from compilers about unused paramter
 * in release mode (ex.: sometimes it is needed only in debug).
 *
 * <code>
 *   const int count=doSth();
 *   assert( count>=0 );    // in release this line is not present
 *   System::ignore(count); // prevent warning - now variable is used
 * </code>
 *
 */
#ifndef INCLUDE_SYSTEM_IGNORE_HPP_FILE
#define INCLUDE_SYSTEM_IGNORE_HPP_FILE

/* public header */

namespace System
{

/** \brief prevents warnings about unsused variable.
 */
template<typename T>
inline void ignore(const T &/*t*/)
{
}

} // namespace System

#endif
