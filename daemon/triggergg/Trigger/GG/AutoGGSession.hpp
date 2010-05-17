/*
 * AutoGGSession.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_AUTOGGSESSION_HPP_FILE
#define INCLUDE_TRIGGER_GG_AUTOGGSESSION_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libgadu.h>

#include "System/AutoVariable.hpp"

// TODO: tests

namespace Trigger
{
namespace GG
{

namespace detail
{
/** \brief helper object for System::AutoVariable<> specialization.
 */
class SessionAutoHolder
{
public:
  /** \brief typedef for element held inside. */
  typedef struct gg_session* TValue;

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
      gg_free_session(v_);
  }

private:
  TValue v_;
}; // class SessionAutoHolder
} // namespace detail


/** \brief session object implementation. */
typedef System::AutoVariable<detail::SessionAutoHolder> AutoGGSession;

} // namespace GG
} // namespace Trigger

#endif
