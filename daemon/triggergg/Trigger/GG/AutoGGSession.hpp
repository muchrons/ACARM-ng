/*
 * AutoGGSession.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_AUTOGGSESSION_HPP_FILE
#define INCLUDE_TRIGGER_GG_AUTOGGSESSION_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libgadu.h>

#include "System/AutoVariable.hpp"

// TODO: comments
// TODO: tests

namespace Trigger
{
namespace GG
{

namespace detail
{
class SessionAutoHolder
{
public:
  typedef struct gg_session* TValue;

  explicit SessionAutoHolder(TValue v):
    v_(v)
  {
  }

  SessionAutoHolder(void):
    v_(NULL)
  {
  }

  TValue get(void) const
  {
    return v_;
  }

  void deallocate(void)
  {
    if(v_!=NULL)
      gg_free_session(v_);
  }

private:
  TValue v_;
}; // class SessionAutoHolder
} // namespace detail


typedef System::AutoVariable<detail::SessionAutoHolder> AutoGGSession;

} // namespace GG
} // namespace Trigger

#endif
