/*
 * StrError.hpp
 *
 */
#ifndef INCLUDE_BASE_STRERROR_HPP_FILE
#define INCLUDE_BASE_STRERROR_HPP_FILE

/* public header */

#include <string>

namespace Base
{

/** \brief create error string from curren errno's value.
 */
class StrError
{
public:
  /** \brief read error message from errno.
   */
  StrError(void);

  /** \brief get error message.
   *  \return error reported bu errno as a string.
   */
  const std::string &get(void) const
  {
    return msg_;
  }

private:
  std::string msg_;
}; // class StrError

} // namespace Base

#endif
