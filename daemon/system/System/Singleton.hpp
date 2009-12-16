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
#include <boost/checked_delete.hpp>
#include <cassert>

#include "System/AtExit.hpp"


namespace System
{

/** \brief Mayer's singleton implementation on template.
 *
 * implementation is enhanced to allow secure usage, allowing
 * creating objects in proper order destroying in reverse order.
 *
 * \note it is recommended to make constructor of class T
 *       private and make Singleton<T> its friend. this ensures
 *       that instances of T won't be made directly by user.
 *
 */
template<typename T>
class Singleton: private boost::noncopyable
{
public:
  /** \brief get pointer to singleton internal type
   *
   *  singleton object is created on heap for increased
   *  generality (in case T would be huge object).
   *
   *  \return object instance pointer. pointer is returned
   *          instead of reference since it is harder to
   *          accidently make obj-copy this way.
   */
  inline static T *get(void)
  {
    static T *t=init(&t);
    assert(t!=NULL &&
           "object's initialization failed (not RAII-compliant?) or "
           "using object after its deallocation");
    return t;
  }

private:
  // special deallocator type that ensures to NULL internal
  // pointer as well.
  class SingletonDeallocator: public AtExitResourceDeallocator
  {
  public:
    // create member when initialized
    explicit SingletonDeallocator(T **dstPtr):
      dstPtr_(dstPtr)
    {
      assert(dstPtr!=NULL);     // check input
      *dstPtr_=new T;           // create pointer
      assert(*dstPtr!=NULL);    // just in case... (RAII assumed)
    }
    // free when exiting
    virtual ~SingletonDeallocator(void)
    {
      assert(*dstPtr_==NULL);           // object should be already freed
      boost::checked_delete(*dstPtr_);  // just in case...
    }
    // deallocation on exit
    virtual void deallocate(void)
    {
      boost::checked_delete(*dstPtr_);  // delete object
      *dstPtr_=NULL;                    // mark pointer as unusable
    }
  private:
    T **dstPtr_;
  }; // class SingletonDeallocator

  // helper method that creates output object. this allows to keep
  // singleton's get() as clean and simple as possible.
  static T *init(T **dstPtr)
  {
    assert(dstPtr!=NULL);
    // register singleton's deallocator
    AtExit::TDeallocPtr dealloc( new SingletonDeallocator(dstPtr) );
    assert( dealloc.get()!=NULL );
    AtExit::registerDeallocator(dealloc);

    // return pointer as a result
    T *t=*dstPtr;
    assert(t!=NULL);
    return t;
  } // init()

  Singleton(void);  // no instances allowed
}; // class Singleton

} // namespace System

#endif

