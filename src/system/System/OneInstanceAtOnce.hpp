/*
 * OneInstanceAtOnce.hpp
 *
 * object that ensures that only one instance of a given object
 * exists at a given moment. if this is not true assertion fails
 * (in debug) or std::logic_error is thrown (in release).
 *
 */
#ifndef INCLUDE_SYSTEM_ONEINSTANCEATONCE_HPP_FILE
#define INCLUDE_SYSTEM_ONEINSTANCEATONCE_HPP_FILE

/* public header */

#include <stdexcept>
#include <cassert>
#include <boost/noncopyable.hpp>

#include "System/Threads/SafeInitLocking.hpp"

namespace System
{

/** \brief class that one instnace of is allowed at a time.
 *  \param T         type to create guard for (CRTP-style).
 *  \param useAssert true if use asserts in debug mode, false otherwise.
 *
 *  this class ensures only one copy of a given type instance can
 *  be present in the system at a given time. if it is not the case
 *  in assertion will fail (debug) or exception will be thrown (release).
 *
 *  to use this class one must derive from it in CRTP-style.
 *
 *  <code>
 *    class MyClass: private OneInstanceAtOnce<MyClass>
 *    {
 *      // ...
 *    };
 *  </code>
 *
 *  the main differences between this class and the singleton is that:
 *    - this class needs not to be global
 *    - second instance can be created, whenfirst one is destroyed.
 *
 */
template<typename T, bool useAssert=true>
class OneInstanceAtOnce: private boost::noncopyable
{
protected:
  /** \brief check if no instances are present.
   */
  OneInstanceAtOnce(void)
  {
    check(true);
  }

  /** \brief release lock - next instance can be created.
   */
  ~OneInstanceAtOnce(void)
  {
    check(false);
  }

private:
  // this method combines two functionalities since it needs to have an access
  // to static bool and mutex at the same time + be unique for each template
  // instanciated time.
  void check(bool isInit)
  {
    SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(mutex);
    Threads::SafeInitLock lock(mutex);
    static bool           exists=false;
    if(isInit)
    {
      if(exists)
      {
        if(useAssert)
          assert(!"attempting to create second instance of this type");
        throw std::logic_error("attempting to create second instance of this type");
      }
      exists=true;
    }
    else // if(isInit)
    {
      assert(exists && "oops - logic error");
      exists=false;
    }
  }
}; // class TempFile

} // namespace System

#endif

