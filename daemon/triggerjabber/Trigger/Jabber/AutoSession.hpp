/*
 * AutoSession.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_AUTOSESSION_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_AUTOSESSION_HPP_FILE

#include <loudmouth/loudmouth.h>

#include "System/AutoVariable.hpp"

namespace Trigger
{
namespace Jabber
{

namespace detail
{
/** \brief helper object for System::AutoVariable<> specialization.
 */
class SessionAutoHolder
{
public:
  /** \brief typedef for element held inside. */
  typedef LmConnection* TValue;

  /** \brief create object from given pointer.
   *  \param v value to be assigned.
   */
  explicit SessionAutoHolder(TValue v):
    v_(v)
  {
  }
  /** \brief create non-initialized object.
   */
  SessionAutoHolder(void):
    v_(NULL)
  {
  }

  /** \brief get saved value.
   *  \return pointer held inside.
   */
  TValue get(void) const
  {
    return v_;
  }
  /** \brief deallocates object held inside.
   */
  void deallocate(void)
  {
    if(v_!=NULL)
    {
      lm_connection_close (v_, NULL);
      lm_connection_unref (v_);
    }
  }

private:
  TValue v_;
}; // class SessionAutoHolder
} // namespace detail


/** \brief session object implementation. */
typedef System::AutoVariable<detail::SessionAutoHolder> AutoSession;

} // namespace Jabber
} // namespace Trigger

#endif
