/*
 * SharedPtrNotNull.hpp
 *
 * this class is copy of System::SharedPtrNotNULL<> that throws difference exception
 * that saves log message in case of error as well.
 *
 */
#ifndef INCLUDE_COMMONS_SHAREDPTRNOTNULL_HPP_FILE
#define INCLUDE_COMMONS_SHAREDPTRNOTNULL_HPP_FILE

/* public header */

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/operators.hpp>
#include <cassert>

#include "Commons/ExceptionUnexpectedNULL.hpp"

namespace Commons
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
  // NOTE: following declarations are here for compatibility with other smart-ptr types:
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
  explicit SharedPtrNotNULL(T *t):
    ptr_(t)
  {
    ensure();
  }
  /** \brief copy c-tor.
   *  \param other object to copy from.
   */
  SharedPtrNotNULL(const SharedPtrNotNULL &other)
  {
    ptr_=other.ptr_;
    assert( ptr_.get()!=NULL );
  }
  /** \brief create object from boost::shared_ptr<>.
   *  \param p pointer to share.
   */
  SharedPtrNotNULL(SharedPtr p):
    ptr_(p)
  {
    ensure();
  }
  /** \brief create object from std::auto_ptr<>.
   *  \param p pointer get ownership of.
   */
  SharedPtrNotNULL(std::auto_ptr<T> p):
    ptr_( p.release() )
  {
    ensure();
  }
  /** \brief conversion operator.
   *  \return boost::shared_ptr<> for a given value.
   */
  operator SharedPtr(void) const
  {
    assert( ptr_.get()!=NULL );
    return ptr_;
  }
  /** \brief less-then compare
   *  \param other element to compare with.
   *  \return return true if other element is greater.
   */
  inline bool operator<(const SharedPtrNotNULL<T> &other) const
  {
    return ptr_<other.ptr_;
  }
  /** \brief assignmen of other instance.
   *  \param other object to assigne from.
   *  \return const-reference to this object.
   */
  const SharedPtrNotNULL<T> &operator=(const SharedPtrNotNULL<T> &other)
  {
    if(&other!=this)
      ptr_=other.ptr_;
    return *this;
  }
  /** \brief arrow operator.
   *  \return const pointer to this.
   */
  const T *operator->(void) const
  {
    return get();
  }
  /** \brief arror operator.
   *  \return pointer to this.
   */
  T *operator->(void)
  {
    return get();
  }
  /** \brief dereference operator.
   *  \return const reference to this.
   */
  const T &operator*(void) const
  {
    return *get();
  }
  /** \brief dereference operator.
   *  \return reference to this.
   */
  T &operator*(void)
  {
    return *get();
  }
  /** \brief test getter.
   *  \return direct pointer value.
   */
  const T *get(void) const
  {
    assert( ptr_.get()!=NULL );
    return ptr_.get();
  }
  /** \brief test const getter.
   *  \return direct pointer value.
   */
  T *get(void)
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
  void swap(SharedPtrNotNULL<T> &other)
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
  void ensure(const T *t) const
  {
    if(t==NULL)
      throw ExceptionUnexpectedNULL(SYSTEM_SAVE_LOCATION);
  }

  SharedPtr ptr_;
}; // struct SharedPtrNotNULL

} // namespace Commons

#endif
