/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_EXCEPTION_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_EXCEPTION_HPP_FILE

#include "Trigger/Exception.hpp"

namespace Trigger
{
namespace SnortSam
{
/** \brief base for all snortsam-trigger related exceptions
 */
class Exception: public Trigger::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    Trigger::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace SnortSam
} // namespace Trigger

#endif
