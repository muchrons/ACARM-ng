/*
 * LessThan.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_LESSTHAN_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_LESSTHAN_HPP_FILE

#include "Preprocessor/Checkers/Mode.hpp"
#include "Preprocessor/Checkers/detail/NumberWrapper.hpp"

namespace Preprocessor
{
namespace Checkers
{

/** \brief class checking two strings with less-than order.
 */
class LessThan: public Mode
{
public:
  /** \brief create checker.
   *  \param str string to be compared on demand.
   */
  explicit LessThan(const std::string &str);

  /** \brief check if given string is less than string given in c-tor.
   *  \param str string to check.
   *  \return true if string is less than given one, false otherwise.
   *  \note if both strings are numbers, comparison is not alphabetical, but numerical.
   */
  virtual bool check(const std::string &str) const;

private:
  const std::string   str_;
  const detail::NumberWrapper num_;
}; // class LessThan

} // namespace Checkers
} // namespace Preprocessor

#endif
