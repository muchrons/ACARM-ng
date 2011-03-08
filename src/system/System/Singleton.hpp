/*
 * Singleton.hpp
 *
 * singleton template for general usage.
 *
 */
#ifndef INCLUDE_SYSTEM_SINGLETON_HPP_FILE
#define INCLUDE_SYSTEM_SINGLETON_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/checked_delete.hpp>
#include <cassert>

#include "System/AtExit.hpp"
#include "System/NoInstance.hpp"
#include "System/Threads/SafeInitLocking.hpp"

namespace System
{
namespace detail
{
/** \brief helper object implementing data holding logic in Singleton.
 *  \param T type to be hold inside the singleton object.
 *
 *  in order to be on the safe side T must be noncopyable. it is also
 *  good idea to make its c-tor private and add friend declaration
 *  for Singleton<T>. to enusre all conditions are met user might want
 *  to derive from System::OneInstanceAtOnde<> template.
 */
template<typename T>
class SingletonData: private boost::noncopyable
{
public:
  /** \brief create from pointer and take ownership.
   *  \param t pointer to take ownership of.
   */
  explicit SingletonData(T *t):
    sp_(t),
    wp_(sp_)
  {
  }
  /** \brief called when deallocation should take place.
   *
   *  this method provided mean of signalizing that AtExit() has been
   *  called and Singleton shoud deallocate object ASAP. it cannot be
   *  deallocated stright away though, since it may be still in use
   *  by other thread, holding Singleton<T>::PointerWrapper instance.
   *
   *  after this method is called, last instance will deallocate all
   *  related data.
   */
  void prepareToDeallocate(void)
  {
    sp_.reset();    // give away ownership assigned to singleton
                    // so that after last releas it can be deleted
  }
  /** \brief gets shared_ptr<T> - pointer to user object itself.
   *  \return shared pointer to user's object.
   *  \note this pointer will be NULL <=> expired()==true.
   */
  boost::shared_ptr<T> get(void)
  {
    return wp_.lock();  // it is important to call lock() here, since
                        // sp_ may no longer point anywere (i.e. after
                        // call to prepareToDeallocate() method).
  }
  /** \brief checks if no instance exists.
   *  \return true, if there are not instances of user object's,
   *          false otherwise.
   */
  bool expired(void) const
  {
    return wp_.expired();
  }

private:
  boost::shared_ptr<T> sp_;
  boost::weak_ptr<T>   wp_;
}; // struct SingletonData
} // namespace detail


/** \brief thread-safe Mayer's/Phoenix singleton implementation template.
 *
 * implementation is enhanced to allow secure usage, allowing
 * creating objects in proper order destroying in reverse order.
 *
 * notice that if order of creation/destruction must be preserved
 * singleton A using singleton B should call B::get() in its c-tor
 * to ensure they are created in proper order.
 *
 * singleton can be safely used before main(), in global constructors
 * and from multiple threads. using it in destructors of global objects
 * (i.e. after leaving main()) is safe too, however one have to keep in
 * mind that this is where Phoenix functionality takes over and so it may
 * re-create object from scratch.
 *
 * \note it is recommended to make constructor of class T
 *       private and make Singleton<T> its friend. this ensures
 *       that instances of T won't be made directly by user.
 *       T should not noncopyable as well.
 *
 */
template<typename T>
class Singleton: private boost::noncopyable,
                 private NoInstance
{
private:
  typedef detail::SingletonData<T>   TSingletonData;

public:
  /** \brief object returned instead of raw user's pointer.
   *
   *  this class ensures that user data will not be deallcated by another
   *  thread while being used. as long as instance of this class exist, user
   *  is ensured that obect is valid as well.
   *
   *  \warning this object MUST NOT be saved by user anywhere! it can be
   *           obtained by get() and should disappear when line ends.
   */
  class PointerWrapper
  {
  private:
    // only singleton is allowed to make instances.
    friend class Singleton<T>;
    /** \brief create new istance.
     *  \param mutex pointer to mutex used for locking some operations.
     *  \param d     data container - when d::expired() is true, it will be
     *               deallocated and pointer will be NULLed.
     */
    PointerWrapper(Threads::SafeInitLock::MutexType  *mutex,
                   detail::SingletonData<T>         **d):
      mutex_(mutex),
      d_(d),
      sp_( (*d_)->get() )   // this is ok, since mutex is locked here
    {
      assert( sp_.get()!=NULL );
      assert( *d_      !=NULL );
      assert( mutex_   !=NULL );
      assert( (*d_)->expired()==false );
    }

  public:
    /** \brief deallocates object.
     *
     *  when no instances of user's data are required any more this
     *  d-tor will deallocate data in thread-safe way.
     */
    ~PointerWrapper(void)
    {
      assert( sp_.get()!=NULL );
      assert( *d_      !=NULL );
      assert( mutex_   !=NULL );

      sp_.reset();
      Threads::SafeInitLock lock(*mutex_);
      if( *d_!=NULL && (*d_)->expired() )
      {
        assert( (*d_)->get().get()==NULL && "problem with expired() call" );
        delete *d_;
        *d_=NULL;
      }
    }
    /** \brief arrow operator for pointer-compatibility.
     *  \return pointer to user raw data.
     */
    T *operator->(void)
    {
      assert( sp_.get()!=NULL );
      assert( *d_      !=NULL );
      assert( mutex_   !=NULL );
      return sp_.get();
    }
    /** \brief conversion operator to user raw pointer.
     *  \return raw poitner to user data.
     */
    operator T*(void)
    {
      assert( sp_.get()!=NULL );
      assert( *d_      !=NULL );
      assert( mutex_   !=NULL );
      return sp_.get();
    }

  private:
    Threads::SafeInitLock::MutexType  *mutex_;
    detail::SingletonData<T>         **d_;
    boost::shared_ptr<T>               sp_;
  }; // class PointerWrapper


  /** \brief get pointer to singleton internal type
   *
   *  singleton object is created on heap for increased
   *  generality (in case T would be huge object).
   *
   *  \return object instance pointer. pointer is returned
   *          instead of reference since it is harder to
   *          accidently make obj-copy this way.
   */
  static PointerWrapper get(void)
  {
    static TSingletonData *d=NULL;
    Threads::SafeInitLock lock( getSingletonMutex() );
    if(d==NULL)
      init(&d);
    assert(d!=NULL && "object's initialization failed");
    assert( d->expired()==false );
    return PointerWrapper( &getSingletonMutex(), &d );
  }

private:
  // special deallocator type that ensures to NULL internal
  // pointer as well.
  class SingletonDeallocator: public AtExitResourceDeallocator
  {
  public:
    // create member when initialized
    SingletonDeallocator(TSingletonData                   **d,
                         Threads::SafeInitLock::MutexType  *mutex):
      d_(*d),
      ptr_(new PointerWrapper(mutex, d) )
    {
      assert(d_        !=NULL);
      assert(ptr_.get()!=NULL);
    }
    // deallocation on exit
    virtual void deallocate(void)
    {
      assert(d_!=NULL);
      d_->prepareToDeallocate();    // mark as not used internally
      d_=NULL;
      ptr_.reset();                 // deallocate data pointer ASAP
    }

  private:
    TSingletonData                    *d_;
    boost::scoped_ptr<PointerWrapper>  ptr_;
  }; // class SingletonDeallocator

  // helper method that creates output object. this allows to keep
  // singleton's get() as clean and simple as possible.
  static void init(TSingletonData **dstPtr)
  {
    assert( dstPtr!=NULL);
    assert(*dstPtr==NULL);
    // register singleton's deallocator
    *dstPtr=new TSingletonData(new T);
    AtExit::TDeallocPtr dealloc( new SingletonDeallocator( dstPtr,
                                                          &getSingletonMutex() ) );
    assert( dealloc.get()!=NULL );
    AtExit::registerDeallocator(dealloc);
    assert(*dstPtr!=NULL);
  } // init()

  // gives access to static mutex, that's always ready to usage.
  static Threads::SafeInitLock::MutexType &getSingletonMutex(void)
  {
    SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(mutex);
    return mutex;
  }
}; // class Singleton

} // namespace System

#endif
