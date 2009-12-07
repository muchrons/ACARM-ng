/*
 * AtExitResourceDealocator.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_ATEXITRESOURCEDEALOCATOR_HPP_FILE
#define INCLUDE_SYSTEM_ATEXITRESOURCEDEALOCATOR_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>
#include <cassert>


namespace System
{

/** \brief resource's deallocator interface.
 *
 * base class for all deallocators that are to be
 * registered with AtExit class.
 */
struct AtExitResourceDeallocator: private boost::noncopyable
{
  /** \brief virtual destructor for polimorphic collections.
   */
  virtual ~AtExitResourceDeallocator(void)
  {
  }
  /** \brief resource deallocating method.
   *
   * method that is called by AtExit when program quits.
   * treat it the same way as destructors - it should NOT
   * throw.
   * it is guaranteed that AtExit will call this method
   * exactly once for each object, when quiting.
   */
  virtual void deallocate(void)=0;
}; // struct AtExitResourceDeallocator


/** \brief template for dealocating pointers to classes.
 */
template<typename T>
class AtExitMemoryDeallocator: public AtExitResourceDeallocator
{
public:
  /** \brief creates dealocator from pointer.
   *  \param t pointer to object to be deleted.
   */
  explicit AtExitMemoryDeallocator(T *t):
    t_(t)
  {
  }
  /** \brief creates dealocator from object's auto pointer.
   *  \param ap auto pointer to object to be deleted.
   */
  explicit AtExitMemoryDeallocator(std::auto_ptr<T> ap):
    t_(ap)
  {
  }
  /** \brief deletes object.
   */
  virtual void deallocate(void)
  {
    t_.reset();
  }
private:
  std::auto_ptr<T> t_;
}; // class AtExitMemoryDeallocator

} // namespace System

#endif

