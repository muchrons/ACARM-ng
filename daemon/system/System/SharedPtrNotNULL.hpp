/*
 * SharedPtrNotNull.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_SHAREDPTRNOTNULL_HPP_FILE
#define INCLUDE_SYSTEM_SHAREDPTRNOTNULL_HPP_FILE

/* public header */

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits/add_const.hpp>
#include <cassert>

#include "System/ExceptionPointerIsNULL.hpp"

namespace System
{

/** \brief boost::shared_ptr-like class that does not permit NULLs.
 *  \note if NULL will be passed to this class it will throw
 *        ExceptionUnexpectedNULL.
 */
template<typename T>
class SharedPtrNotNULL: public boost::less_than_comparable< SharedPtrNotNULL<T> >,
                        public boost::equivalent<           SharedPtrNotNULL<T> >,
                        public boost::equality_comparable<  SharedPtrNotNULL<T> >,
                        public boost::equality_comparable<  SharedPtrNotNULL<T>, boost::shared_ptr<T> >
{
public:
  /** \brief type used as boost::shared_ptr<>. */
  typedef boost::shared_ptr<T>             SharedPtr;
  /** \brief type of this object. */
  typedef SharedPtrNotNULL<T>              this_type;
  /** \brief type of element held inside (for compatibility with boost::shared_ptr). */
  typedef typename SharedPtr::element_type element_type;
  /** \brief type of element held inside (for compatibility with boost::shared_ptr). */
  typedef typename SharedPtr::value_type   value_type;
  /** \brief type of pointer to element held inside (for compatibility with boost::shared_ptr). */
  typedef typename SharedPtr::pointer      pointer;
  /** \brief type of reference to element held inside (for compatibility with boost::shared_ptr). */
  typedef typename SharedPtr::reference    reference;

  /** \brief create class from raw-pointer.
   *  \param t pointer to take ownership of.
   */
  explicit SharedPtrNotNULL(pointer t):
    ptr_(t)
  {
    ensure();
  }
  /** \brief copy c-tor.
   *  \param other object to copy from.
   */
  template<typename U>
  SharedPtrNotNULL(const SharedPtrNotNULL<U> &other)
  {
    ptr_=other.shared_ptr();
    assert( ptr_.get()!=NULL );
  }
  /** \brief create object from boost::shared_ptr<>.
   *  \param p pointer to share.
   */
  template<typename U>
  SharedPtrNotNULL(const boost::shared_ptr<U> &p):
    ptr_(p)
  {
    ensure();
  }
  /** \brief create object from std::auto_ptr<>.
   *  \param p pointer get ownership of.
   */
  template<typename U>
  SharedPtrNotNULL(std::auto_ptr<U> p):
    ptr_( p.release() )
  {
    ensure();
    assert( p.get()==NULL );
  }
  /** \brief conversion to boost::shared_ptr<>.
   *  \return boost::shared_ptr<> for a given value.
   */
  SharedPtr shared_ptr(void) const
  {
    assert( ptr_.get()!=NULL );
    return ptr_;
  }
  /** \brief less-then compare
   *  \param other element to compare with.
   *  \return true if this<other, false otherwise.
   */
  inline bool operator<(const this_type &other) const
  {
    return ptr_<other.ptr_;
  }
  /** \brief assignmen of other instance.
   *  \param other object to assigne from.
   *  \return const-reference to this object.
   */
  template<typename U>
  typename boost::add_const<this_type&>::type operator=(const SharedPtrNotNULL<U> &other)
  {
    if( other.get()!=this->get() )
      ptr_=other.shared_ptr();
    return *this;
  }
  /** \brief arrow operator.
   *  \return const pointer to this.
   */
  typename boost::add_const<pointer>::type operator->(void) const
  {
    return get();
  }
  /** \brief arror operator.
   *  \return pointer to this.
   */
  pointer operator->(void)
  {
    return get();
  }
  /** \brief dereference operator.
   *  \return const reference to this.
   */
  typename boost::add_const<reference>::type operator*(void) const
  {
    return *get();
  }
  /** \brief dereference operator.
   *  \return reference to this.
   */
  reference operator*(void)
  {
    return *get();
  }
  /** \brief test getter.
   *  \return direct pointer value.
   */
  typename boost::add_const<pointer>::type get(void) const
  {
    assert( ptr_.get()!=NULL );
    return ptr_.get();
  }
  /** \brief test const getter.
   *  \return direct pointer value.
   */
  pointer get(void)
  {
    assert( ptr_.get()!=NULL );
    return ptr_.get();
  }
  /** \brief swaps pointers held inside.
   *  \param other object to swap pointers with.
   *  \note this call is NOT thread safe, and make others not-thread-sage too!
   *        if this call is to be used, whole access has to be carefully
   *        mutexed, allong with all places given object is used in.
   */
  void swap(this_type &other)
  {
    assert( get()!=NULL );
    ptr_.swap(other.ptr_);
    assert( get()!=NULL );
  }

private:
  void ensure(void) const
  {
    ensure( ptr_.get() );
  }
  void ensure(const typename boost::add_const<pointer>::type t) const
  {
    if(t==NULL)
      throw ExceptionPointerIsNULL(SYSTEM_SAVE_LOCATION, "t");
  }

  SharedPtr ptr_;
}; // struct SharedPtrNotNULL

} // namespace System

#endif
