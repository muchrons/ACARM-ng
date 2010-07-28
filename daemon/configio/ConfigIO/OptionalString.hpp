/*
 * OptionalString.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_OPTIONALSTRING_HPP_FILE
#define INCLUDE_CONFIGIO_OPTIONALSTRING_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/Exception.hpp"

//TODO LimitedNullString can't be used instead of creating a new entity?

namespace ConfigIO
{
/** \brief string that may not be set, if needed.
 */
class OptionalString
{
public:
  /** \brief exception thrown when string was not set, but value was requested.
   */
  struct ExceptionStringNotSet: public Exception
  {
    /** \brief create expcetion.
     *  \param where location where exception has been rised.
     */
    explicit ExceptionStringNotSet(const Location &where):
      Exception(where, "string not set")
    {
    }
  }; // struct ExceptionStringNotSet


  /** \brief create with string not set.
   */
  OptionalString(void):
    isSet_(false)
  {
  }
  /** \brief create string.
   *  \param str string's value.
   */
  template<typename T>
  OptionalString(const T &str):
    isSet_(true),
    value_(str)
  {
  }
  /** \brief checks if string is set or not.
   *  \return true if string has been set, false otherwise.
   */
  bool isSet(void) const
  {
    return isSet_;
  }
  /** \brief gets string's value or throws if not set.
   *  \return string's value that has been set.
   */
  const std::string &get(void) const
  {
    if( !isSet() )
      throw ExceptionStringNotSet(SYSTEM_SAVE_LOCATION);
    return value_;
  }

private:
  bool        isSet_;
  std::string value_;
}; // class OptionalString

} // namespace ConfigIO

#endif
