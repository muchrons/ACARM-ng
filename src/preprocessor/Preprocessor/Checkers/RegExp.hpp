/*
 * RegExp.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_REGEXP_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_REGEXP_HPP_FILE

#include <boost/regex.hpp>

#include "Preprocessor/Checkers/Mode.hpp"

namespace Preprocessor
{
namespace Checkers
{

/** \brief checker using regular expressions for comparison.
 */
class RegExp: public Mode
{
public:
  /** \brief create instance.
   *  \param expr regular expression to check against.
   */
  explicit RegExp(const std::string &expr);

  /** \brief check if given string matches defined regular expression.
   *  \param str string to check.
   *  \return true if string matches regular expression, false otherwise.
   */
  virtual bool check(const std::string &str) const;

private:
  boost::regex re_;
}; // class RegExp

} // namespace Checkers
} // namespace Preprocessor

#endif
