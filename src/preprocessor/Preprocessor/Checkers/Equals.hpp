/*
 * Equals.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_EQUALS_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_EQUALS_HPP_FILE

#include "Preprocessor/Checkers/Mode.hpp"

namespace Preprocessor
{
namespace Checkers
{

/** \brief class checking two strings in terms of equality.
 */
class Equals: public Mode
{
public:
  /** \brief create checker.
   *  \param str string to be compared on demand.
   */
  explicit Equals(const std::string &str);

  // TODO: description copied from Contains class
  /** \brief check if given string contains string given in c-tor.
   *  \param str string to check.
   *  \return true if string is part of given on, false otherwise.
   */
  virtual bool check(const std::string &str) const;

private:
  const std::string str_;
}; // class Equals

} // namespace Checkers
} // namespace Preprocessor

#endif
