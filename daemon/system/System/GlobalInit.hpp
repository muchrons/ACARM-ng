/*
 * GlobalInit.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_GLOBALINIT_HPP_FILE
#define INCLUDE_SYSTEM_GLOBALINIT_HPP_FILE

/* public header */

#include "System/Threads/SafeInitLocking.hpp"
#include "System/AtExit.hpp"
#include "System/ignore.hpp"

namespace System
{

namespace detail
{
/** \brief helper function giving access to global mutex.
 *  \return reference to global mutex used for locking GlobalInit() internals.
 */
Threads::SafeInitLock::MutexType &getGlabalInitMutex(void);
} // namespace detail

/**
 * \brief template wrapper for global (de)initilization oof data.
 *
 * number of libraries uses global data that are not initialized
 * in the proper way, sometimes are not uninitializaed at all, initalizaiton
 * is not thread safe, etc... this wrapper solves this issues.
 *
 * this class initializes user object in thread-safe way, and uninitializes
 * it uppon program exit. when it is needed once more, it will be
 * re-initialized and deallocated stright away.
 *
 * used-defined T type has to provide static methods init() and uninit()
 * that performs as-is initialization (i.e. no locking, checks, etc... are
 * required - it is all done by GlobalInit<> template) and uninitialization
 * of given element, respecitively.
 * when init() throws, uninit() is never called.  uninit() must NOT throw!
 *
 * init() is ensured to be called exactly once, during the first object's
 * creation (thread safe, with valid object creation order preserved).
 * uninit() is ensured to be called exactly once (if init() didn't throw)
 * when program exits. it is ensured that uninit() will NOT be called
 * until main() exits and last object holds GlobalInit<> copy. notice however
 * that when new GlobalInit<> instances will be created after leaving main()
 * it may happen that init()+uninit() sequence will be called more than once,
 * in proper order.
 *
 * typical usage in code goes as follows:
 * <code>
 *   namespace detail
 *   {
 *     struct MyLibInitImpl
 *     {
 *       static void init(void)
 *       {
 *         // initialization code for MyLib here
 *       }
 *       static void uninit(void)
 *       {
 *         // uninitialization code for MyLib here
 *       }
 *     };
 *   }
 *   typedef System::GlobalInit<detail::MyLibInitImpl> MyLibInit;
 *   // ...
 *   MyLibInit mylibinit;
 *   // from this point in time MyLib is initialized and can be used.
 *   // ...
 * </code>
 *
 * notice that it is good practice to keep GlobalInit<> instance as a private
 * object member for those elements that relay on given library to be initialized.
 */
template<typename T>
class GlobalInit
{
private:
  typedef enum
  {
    STATE_UNINITIALIZED,    // not yet initialized
    STATE_INITIALIZED,      // init() called
    STATE_ATEXIT_END        // atexit() finished but was unable to uninit() (still in use)
  } State;

public:
  /** \brief create handle object.
   *
   *  initializes library, if not already initialized. increases usage count.
   */
  GlobalInit(void)
  {
    Threads::SafeInitLock lock( mutex() );
    if( mark(lock)==STATE_UNINITIALIZED )
    {
      assert( counter(lock)==0 );
      GlobalUninit        *tmp=new GlobalUninit();
      AtExit::TDeallocPtr  uninit(tmp);     // put to auto-ptr
      AtExit::registerDeallocator(uninit);  // register uninitializer
      T::init();                            // initialize user object
      mark(lock)=STATE_INITIALIZED;         // mark successfull initialization
    }
    // mark usage
    ++counter(lock);
  }

  /** \brief copy constructor.
   *
   *  increases reference count.
   */
  GlobalInit(const GlobalInit &/*other*/)
  {
    Threads::SafeInitLock lock( mutex() );
    assert( counter(lock)>0 );
    assert( mark(lock)!=STATE_UNINITIALIZED );
    // mark usage
    ++counter(lock);
  }

  /** \brief assignment operator.
   *  \return const-reference to this object.
   */
  const GlobalInit &operator=(const GlobalInit &/*other*/)
  {
    Threads::SafeInitLock lock( mutex() );
    assert( counter(lock)>0 );
    assert( mark(lock)!=STATE_UNINITIALIZED );
    // NOTE: nothing has to be done here, since each copy is equivalent to
    //       other (no private fields) and overwriting one instance with
    //       another does not change total objects count (i.e. object being
    //       assigned to already exists).
    return *this;
  }

  /** \brief destroy object being referenced.
   *
   *  decreases reference count. if this is last instance holding reference
   *  and atexit() is already done, this will uninitialize library as well.
   */
  ~GlobalInit(void)
  {
    Threads::SafeInitLock lock( mutex() );
    assert( counter(lock)>0 );
    assert( mark(lock)!=STATE_UNINITIALIZED );
    // mark usage
    --counter(lock);
    // if this is the last instance and atexit() has been already
    // finished we have to deallocate ourselves
    if( counter(lock)==0 && mark(lock)==STATE_ATEXIT_END )
      uninit(lock);
  }

private:
  // helper class to uninitialize given library
  struct GlobalUninit: public AtExitResourceDeallocator
  {
    // deallocation itself
    virtual void deallocate(void)
    {
      Threads::SafeInitLock lock( mutex() );
      if( mark(lock)==STATE_UNINITIALIZED ) // something failed during initialization - do nothing
        return;
      if( counter(lock)==0 )                // only if no one is using this any more
        uninit(lock);                       // if not used, uninitialize it
      else
        mark(lock)=STATE_ATEXIT_END;        // makr that atexit() didn't called uninit()
    }
  }; // struct GlobalUninit

  // get per-instance mutex (required to allow chain-initialization)
  static Threads::SafeInitLock::MutexType &mutex(void)
  {
    // it has to be locked by global mutex to ensure valid initialization
    // before first usage
    Threads::SafeInitLock lock( detail::getGlabalInitMutex() );
    // this is static inside
    SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(mutex);
    return mutex;
  }
  // this must be done within a lock to ensure safe initialization
  static unsigned long &counter(Threads::SafeInitLock &)
  {
    static unsigned long cnt=0;
    return cnt;
  }
  // mark that atexit handle has already finished
  static State &mark(Threads::SafeInitLock &)
  {
    static State mark=STATE_UNINITIALIZED;
    return mark;
  }
  // performs deallocation
  static void uninit(Threads::SafeInitLock &lock)
  {
    ignore(lock);
    assert( counter(lock)==0 );
    assert( mark(lock)!=STATE_UNINITIALIZED );

    T::uninit();                        // not used, so can be uninitialized
    mark(lock)=STATE_UNINITIALIZED;     // mark deallocation
  }
}; // class GlobalInit

} // namespace System

#endif
