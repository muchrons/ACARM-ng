/*
 * Location.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONS_LOCATION_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONS_LOCATION_HPP_FILE

/* public header */

#include <string>

#include "System/FunctionName.hpp"

namespace System
{
namespace Exceptions
{
namespace detail
{

/** \brief private, implementaion class to save invormation wher called.
 */
class Location
{
public:
  /** \brief creates location objet information.
   *  \param file     file name.
   *  \param line     line number in file.
   *  \param function function (pretty) name.
   *  \note all const char* paramters are assumed to be compile-time defined
   *        strings and so only pointer copies are being made.
   */
  Location(const char   *file,
           unsigned int  line,
           const char   *function);
  /** \brief converts data to string.
   *  \return formated string with full location pieces of information.
   */
  const std::string &getStr(void) const
  {
    return str_;
  }

private:
  std::string str_;
}; // class Location

} // namespace detail
} // namespace Exceptions
} // namespace System

#endif
