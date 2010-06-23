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

/**
 * @brief Helper class to provide automatic deallocation of idmef_message_t
 */
class IdmefMessageHolder
{
public:
  /**
   * @brief local typedef
   */
  typedef idmef_message_t* TValue;

  /**
   * @brief c-tor Creates object from idmef_message_t
   */
  explicit IdmefMessageHolder(TValue v):
    v_(v)
  {
  }

  /**
   * @brief default c-tor, initializes to null
   */
  IdmefMessageHolder(void):
    v_(NULL)
  {
  }

  /**
   * @brief get value without ownership
   */
  TValue get(void) const
  {
    return v_;
  }

  /**
   * @brief deallocate idmef_message_t object
   */
  void deallocate(void)
  {
    if (v_==NULL)
      return;
    idmef_message_destroy(v_);
    v_=NULL;
  }

private:
  TValue v_;
};

} //detail
} //Prelude
} //Input

#endif //IDMEF_MESSAGE_HOLDER_HPP_FILE
