/*
 * IdmefMessageHolder.hpp
 *
 */
#ifndef IDMEF_MESSAGE_HOLDER_HPP_FILE
#define IDMEF_MESSAGE_HOLDER_HPP_FILE

#include <prelude-client.h>
#include <prelude.h>
#include <System/AutoVariable.hpp>
#include "Input/Prelude/GlobalLibPreludeInit.hpp"
#include "Input/Prelude/LogCallback.hpp"
#include "Input/Exception.hpp"

namespace Input
{
namespace Prelude
{
namespace detail
{

class IdmefMessageHolder
{
public:
  typedef idmef_message_t* TValue;

  explicit IdmefMessageHolder(TValue v):
    v_(v)
  {
  }

  IdmefMessageHolder(void):
    v_(NULL)
  {
  }

  TValue get(void) const
  {
    return v_;
  }

  void deallocate(void)
  {
    if (v_==NULL)
      return;
    //refcount<-1
    idmef_message_destroy(v_);
  }

private:
  TValue v_;
};

} //detail
} //Prelude
} //Input

#endif //IDMEF_MESSAGE_HOLDER_HPP_FILE
