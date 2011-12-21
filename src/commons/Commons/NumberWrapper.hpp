/*
 * NumberWrapper.hpp
 *
 */
#ifndef INCLUDE_COMMONS_NUMBERWRAPPER_HPP_FILE
#define INCLUDE_COMMONS_NUMBERWRAPPER_HPP_FILE

/* public header */

#include <string>
#include <cassert>

namespace Commons
{

/** \brief class being wrapper for numeric types, represented as strings.
 */
class NumberWrapper
{
public:
  /** \brief create wrapper object for a given string.
   *  \param str string to be processed.
   */
  explicit NumberWrapper(const std::string &str);

  /** \brief returns information if string passed as an c-tor's argument is a number.
   */
  bool isNumber(void) const
  {
    return isNum_;
  }
  /** \brief gives access to numerical value of a given string, if it is a number.
   *  \return numerical value of a parsed string.
   *  \warning this call cannot be called when given string is not a number.
   */
  double get(void) const
  {
    assert( isNumber() && "getting value of non-number is undefined (logical error)" );
    return value_;
  }

private:
  bool isNumber(const std::string &str) const;

  bool   isNum_;
  double value_;
}; // class NumberWrapper

} // namespace Commons

#endif
