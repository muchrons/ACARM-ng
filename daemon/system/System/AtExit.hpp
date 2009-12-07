/*
 * AtExit.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_ATEXIT_HPP_FILE
#define INCLUDE_SYSTEM_ATEXIT_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

#include "System/AtExitResourceDeallocator.hpp"
#include "System/Exception.hpp"


namespace System
{

/**
 * \brief object-oriented atexit() system call wrapper.
 *
 * when using atexit() it can be seen that it is problematic
 * in 2 ways:
 *   1) POSIX says that there has to be possibility to
 *      register at least 32 handlers. this is not much...
 *   2) it requires to register extern "C" function pointer which
 *      is not scalable since it does not allow registration of
 *      multiple handlers of the same type, but operating on
 *      diffrent resources.
 *
 * in order to overcome this limitations OO AtExit has been provided.
 * it allows to register as many structures as system memory can
 * handle. it also gets object instance as argument, so it is simple
 * to register calls for instances of other objects (not only types).
 *
 * class behaves as singleton (via registerDealocator() method)
 * and is thread-safe through whole program life-time.
 *
 */
class AtExit: private boost::noncopyable
{
public:
  /** \brief pointer to be passed to register call.
   */
  typedef std::auto_ptr<AtExitResourceDeallocator> TDeallocPtr;
  /** \brief registers next handler.
   * registers inside internal structures object that will
   * dealocate resource during application's exit. it takes
   * ownership of dealocator.
   * \param p smart pointer to dealocator to be registered.
   */
  static void registerDeallocator(TDeallocPtr p);

private:
  AtExit(void);

  static void init(void);
  static void registerInternal(TDeallocPtr p);
}; // class AtExit

} // namespace System

#endif

