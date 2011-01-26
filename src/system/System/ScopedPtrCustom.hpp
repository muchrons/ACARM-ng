/*
 * ScopedPtrCustom.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_SCOPEDPTRCUSTOM_HPP_FILE
#define INCLUDE_SYSTEM_SCOPEDPTRCUSTOM_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>
#include <cassert>

namespace System
{

/** \brief boost::scoped_ptr<> equivalent for types with custom deallocators.
 *  \param T           type of element, that pointer to will be held.
 *  \param deallocator deallocating call.
 *
 *  this class is especially usefull when cooperating with C-code that
 *  uses custom allocators and deallocators for its structures.
 *  example usage:
 *  \code
 *    extlib_struct *s=extlib_create(...);
 *    ScopedPtrCustom<extlib_struct, extlib_destroy> ptr(s);
 *    // extlib_destroy(s) will be called uppon scope leaving.
 *  \endcode
 *
 *  \note is is guaranteed that 'deallocator' will be never called on a
 *        NULL pointer, so it can be not-NULL-safe.
 */
template<typename T, void(*deallocator)(T *t)>
class ScopedPtrCustom: private boost::noncopyable,
                       public  boost::less_than_comparable< ScopedPtrCustom<T, deallocator> >,
                       public  boost::equivalent<           ScopedPtrCustom<T, deallocator> >,
                       public  boost::equality_comparable<  ScopedPtrCustom<T, deallocator> >

{
public:
  /** \brief type of this object. */
  typedef ScopedPtrCustom<T, deallocator> this_type;
  /** \brief type of element held inside. */
  typedef T                               element_type;
  /** \brief type of element held inside. */
  typedef T                               value_type;
  /** \brief type of pointer to element held inside. */
  typedef T*                              pointer;
  /** \brief type of reference to element held inside. */
  typedef T&                              reference;

  /** \brief gets ownership of given object.
   *  \param t pointer to take ownership of.
   */
  explicit ScopedPtrCustom(pointer t=NULL):
    t_(t)
  {
  }
  /** \brief deallocates pointer held inside.
   */
  ~ScopedPtrCustom(void)
  {
    deallocate();
  }

  /** \brief comapre pointers.
   *  \param other element to compare with.
   *  \return true if this pointer is less then other, false otherwise.
   */
  bool operator<(const this_type &other) const
  {
    return get()<other.get();
  }
  /** \brief arrow operator.
   *  \return pointer to this.
   */
  pointer operator->(void) const
  {
    return get();
  }
  /** \brief dereference operator.
   *  \return reference to this.
   */
  reference operator*(void) const
  {
    return *get();
  }

  /** \brief gets pointer held inside - const version.
   *  \return owned pointer.
   */
  pointer get(void) const
  {
    return t_;
  }
  /** \brief swap content of this and other pointers.
   *  \param other element to swap content with.
   */
  void swap(this_type &other)
  {
    pointer tmp=other.release();
    assert( other.get()==NULL );
    other.t_=get();
    t_=tmp;
  }
  /** \brief releases pointer held inside.
   *  \return pointer, that is no longer owned by this object.
   */
  pointer release(void)
  {
    pointer tmp=t_;
    t_=NULL;
    return tmp;
  }
  /** \brief set new pointer's value.
   *  \param t new value to be set.
   */
  void reset(pointer t=NULL)
  {
    deallocate();
    t_=t;
  }

private:
  void deallocate(void)
  {
    if(t_!=NULL)
    {
      (*deallocator)(t_);
      t_=NULL;
    }
    assert(t_==NULL);
  }

  pointer t_;
}; // class ScopedPtrCustom

} // namespace System

#endif
