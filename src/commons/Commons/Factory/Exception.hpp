/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_EXCEPTION_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace Commons
{
namespace Factory
{

/** \brief common's factory base exception class.
 */
class Exception: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception was rised.
   *  \param msg   message to represent.
   */
  template<typename T>
  explicit Exception(const char *where, const T &msg):
    Commons::Exception(where, msg)
  {
  }
}; // class Exception

}; // namespace Commons

#endif
