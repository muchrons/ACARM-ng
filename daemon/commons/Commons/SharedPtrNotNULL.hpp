/*
 * SharedPtrNotNull.hpp
 *
 */
#ifndef INCLUDE_COMMONS_SHAREDPTRNOTNULL_HPP_FILE
#define INCLUDE_COMMONS_SHAREDPTRNOTNULL_HPP_FILE

/* public header */

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
class SharedPtrNotNULL: public boost::equality_comparable< SharedPtrNotNULL<T> >
{
public:
  /** \brief type used as boost::shared_ptr<>. */
  typedef boost::shared_ptr<T> PtrT;

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
  SharedPtrNotNULL(PtrT p):
    ptr_(p)
  {
    ensure();
  }
  /** \brief convertion operator.
   *  \return boost::shared_ptr<> for a given value.
   */
  operator PtrT(void)
  {
    assert( ptr_.get()!=NULL );
    return ptr_;
  }
  /** \brief compare this pointer to other.
   *  \param other element to compare with.
   */
  bool operator==(const PtrT &other) const
  {
    return ptr_==other;
  }
  /** \brief compare this pointer to other.
   *  \param other element to compare with.
   */
  bool operator==(const SharedPtrNotNULL<T> &other) const
  {
    return ptr_==other.ptr_;
  }
  /** \brief assignmen of other instance.
   *  \param other object to assigne from.
   *  \return const-reference to this object.
   */
  const SharedPtrNotNULL<T> &operator=(const SharedPtrNotNULL<T> &other)
  {
    if(&other!=this)
    {
      ptr_=other.ptr_;
    }
    return *this;
  }
  /** \brief assignment form boost::shared_ptr<>.
   *  \param other pointer to assign.
   *  \return const-reference to this.
   */
  const SharedPtrNotNULL<T> &operator=(const PtrT &other)
  {
    ensure( other.get() );
    ptr_=other;
    return *this;
  }
  /** \brief arrow operator.
   *  \return const pointer to this.
   */
  const T *operator->(void) const
  {
    assert( ptr_.get()!=NULL );
    return ptr_.get();
  }
  /** \brief arror operator.
   *  \return pointer to this.
   */
  T *operator->(void)
  {
    assert( ptr_.get()!=NULL );
    return ptr_.get();
  }
  /** \brief dereference operator.
   *  \return const reference to this.
   */
  const T &operator*(void) const
  {
    assert( ptr_.get()!=NULL );
    return *ptr_;
  }
  /** \brief dereference operator.
   *  \return reference to this.
   */
  T &operator*(void)
  {
    assert( ptr_.get()!=NULL );
    return *ptr_;
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
    assert( ptr_.get()!=NULL );
    ptr_.swap(other.ptr_);
    assert( ptr_.get()!=NULL );
  }

private:
  void ensure(void) const
  {
    ensure( ptr_.get() );
  }
  void ensure(const T *t) const
  {
    if(t==NULL)
      throw ExceptionUnexpectedNULL(__FILE__);
  }

  PtrT ptr_;
}; // struct SharedPtrNotNULL



/** \brief compares boost_ptr to wrapped_ptr.
 *  \param t1 boost opinter.
 *  \param t2 this type pointer.
 *  \return true if pointers are equal, false otherwise.
 */
template<typename T>
inline bool operator==(const typename SharedPtrNotNULL<T>::PtrT &t1,
                       const          SharedPtrNotNULL<T>       &t2)
{
  return t1.get()==t2.get();
} // operator==()

/** \brief compares boost_ptr to wrapped_ptr.
 *  \param t1 boost opinter.
 *  \param t2 this type pointer.
 *  \return true if pointers are not equal, false otherwise.
 */
template<typename T>
inline bool operator!=(const typename SharedPtrNotNULL<T>::PtrT &t1,
                       const          SharedPtrNotNULL<T>       &t2)
{
  return !(t1==t2);
} // operator!=()

} // namespace Commons

#endif
