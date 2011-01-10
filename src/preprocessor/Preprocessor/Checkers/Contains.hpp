/*
 * Contains.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_CONTAINS_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_CONTAINS_HPP_FILE

#include "Preprocessor/Checkers/Mode.hpp"

namespace Preprocessor
{
namespace Checkers
{

/** \brief checker if given string contains given fragment.
 */
class Contains: public Mode
{
public:
  /** \brief create instance.
   *  \param str string to be checked for presence.
   */
  explicit Contains(const std::string &str);

  /** \brief check if given string contains string given in c-tor.
   *  \param str string to check.
   *  \return true if string is part of given on, false otherwise.
   */
  virtual bool check(const std::string &str) const;

private:
  const std::string str_;
}; // class Contains

} // namespace Checkers
} // namespace Preprocessor

#endif
