/*
 * AutoCptr.hpp
 *
 * class behaving like an auto_ptr<> for C-allocated pointer.
 * it does not use System::AutoVariable<> since it has to be
 * template itself - simple typedef is not enought here.
 *
 */
#ifndef INCLUDE_SYSTEM_AUTOCPTR_HPP_FILE
#define INCLUDE_SYSTEM_AUTOCPTR_HPP_FILE

/* public header */

#include <stdio.h>
#include <unistd.h>
#include <boost/operators.hpp>

namespace System
{

/** \brief auto_ptr<> like class for C-style pointers (malloc/free).
 */
template<typename T>
class AutoCptr: public boost::equality_comparable< AutoCptr<T> >
{
public:
  /** \brief pointer type.
   */
  typedef T* Ptr;
  /** \brief helper object for internal usage only.
   */
  class Helper
  {
  private:
    /** \brief create with value.
     *  \param p pointe rto hold.
     */
    explicit Helper(Ptr p=NULL):
      p_(p)
    {
    }
    /** \brief pointer held inside.
     *  \note class is designed for internal usage only, therefore
     *        hermetization is not an issue here.
     */
    Ptr p_;
    // this blocks usage of internall class by others.
    friend class AutoCptr<T>;
  }; // struct Helper

  /** \brief constructs class from user data-holder.
   *  \param h user data holder.
   */
  AutoCptr(Helper h):
    h_(h)
  {
  }

  /** \brief constructs class from user-data.
   *  \param p raw user data.
   */
  AutoCptr(Ptr p):
    h_(p)
  {
  }

  /** \brief default constructor, creating empty-holding object.
   */
  AutoCptr(void)
  {
  }

  /** \brief copy constructor.
   *  \param ap class to take ownership from.
   */
  AutoCptr(AutoCptr<T> &ap):
    h_(ap.h_)
  {
    ap.invalidate();
  }

  /** \brief destructor.
   */
  ~AutoCptr(void)
  {
    deallocate();
  }

  /** \brief returns user data type.
   *  \return user data in raw form.
   */
  Ptr get(void) const
  {
    return h_.p_;
  }

  /** \brief dealocates resource held and stores new one.
   *  \param p new value to be stored.
   */
  void reset(Ptr p)
  {
    deallocate();
    h_=Helper(p);
  }

  /** \brief gives back ownership of resource without deallocating it.
   *  \return resource that has been released.
   */
  Ptr release(void)
  {
    Ptr p=h_.p_;
    invalidate();
    return p;
  }

  /** \brief conversion operator to helper object.
   *  \returns temporary object with ownership.
   */
  operator Helper(void)
  {
    const Helper h=h_;
    invalidate();
    return h;
  }

  /** \brief assignment operator.
   *  \param h helper object ot assign from.
   *  \returns reference to current object.
   */
  AutoCptr<T>& operator=(Helper h)
  {
    deallocate();
    h_=h;
    return *this;
  }

  /** \brief assignment operator.
   *  \param ap object to take ownership from.
   *  \returns reference to current object.
   */
  AutoCptr<T>& operator=(AutoCptr<T> &ap)
  {
    deallocate();
    h_=ap.h_;
    ap.invalidate();
    return *this;
  }

  /** \brief checks if object is initialized.
   *  \return true if object is initialized.
   */
  bool isInitialized(void) const
  {
    return h_.p_!=NULL;
  }

private:
  void invalidate(void)
  {
    h_=Helper();
  }
  void deallocate(void)
  {
    if( isInitialized() )
    {
      free(h_.p_);
      invalidate();
    }
  }

  Helper h_;
}; // class AutoCptr

} // namespace System

#endif
