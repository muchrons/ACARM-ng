/*
 * AutoVariable.hpp
 *
 * auto_ptr<>-like class wrapper, for given user
 * data-specific thin-wrapper. it allows to easy
 * construction of ownership-passing classes via
 * inheritance.
 *
 */
#ifndef INCLUDE_SYSTEM_AUTOVARIABLE_HPP_FILE
#define INCLUDE_SYSTEM_AUTOVARIABLE_HPP_FILE

/* public header */

#include "System/Exception.hpp"

namespace System
{
/** \brief this is class holding user data type T, to
 *         behave like std::auto_ptr<> when copying.
 *
 *  this is example code showing how to write
 *  data-specific classes. assume MyT to be class
 *  to be held (notice that it does NOT deallocate()'s
 *  object itself!).
 *
 *  \code
 *
 *   class SomeAutoHolder
 *   {
 *   public:
 *     typedef MyT TValue;
 *
 *     explicit SomeAutoHolder(TValue v);
 *     SomeAutoHolder(void);    // create uninitialized object.
 *
 *     TValue get(void) const;  // return current value.
 *     void deallocate(void);   // dealocate resource being held.
 *                              // this method must be resistant to
 *                              // call on uninitialized value _v!
 *   private:
 *     TValue v_;
 *   };
 *
 *  \endcode
 *
 */
template<typename T>
class AutoVariable
{
public:
  /** \brief declaration to make file easier. :)
   */
  typedef typename T::TValue TValue;

  /** \brief constructs class from user data-holder.
   *  \param t user data holder.
   */
  AutoVariable(T t):
    t_(t)
  {
  }

  /** \brief constructs class from user-data.
   *  \param v raw user data.
   */
  AutoVariable(TValue v):
    t_(v)
  {
  }

  /** \brief default constructor, creating empty-holding
   *         object.
   */
  AutoVariable(void)
  {
  }

  /** \brief copy constructor.
   *  \param av class to take ownership from.
   */
  AutoVariable(AutoVariable &av):
    t_(av.t_)
  {
    av.invalidate();    // ownership has been already passed
  }

  /** \brief destructor.
   */
  ~AutoVariable(void)
  {
    t_.deallocate();
  }

  /** \brief returns user data type.
   *  \return user data in raw form.
   */
  TValue get(void) const
  {
    return t_.get();
  }

  /** \brief deallocates resource held and stores new one.
   *  \param v new value to be stored.
   */
  void reset(TValue v)
  {
    t_.deallocate();
    t_=T(v);
  }

  /** \brief gives back ownership of resource without deallocating it.
   *  \return resource that has been released.
   */
  TValue release(void)
  {
    TValue v=t_.get();
    invalidate();
    return v;
  }

  /** \brief conversion operator to helper object.
   *  \returns temporary object with ownership.
   */
  operator T(void)
  {
    T t=t_;
    invalidate();
    return t;
  }

  /** \brief assignment operator.
   *  \param t helper object to assign from.
   *  \returns reference to current object.
   */
  AutoVariable& operator=(T t)
  {
    t_.deallocate();
    t_=t;
    return *this;
  }

  /** \brief assignment operator.
   *  \param av object to take ownership from.
   *  \returns reference to current object.
   */
  AutoVariable& operator=(AutoVariable &av)
  {
    t_.deallocate();
    t_=av.t_;
    av.invalidate();
    return *this;
  }

  /** \brief checks if object is initialized.
   *  \return true if object is initialized.
   */
  bool isInitialized(void) const
  {
    const T invalid;
    return t_.get()!=invalid.get();
  }

private:
  void invalidate(void)
  {
    t_=T();
  }

  T t_;
}; // class AutoVariable

} // namespace System

#endif

