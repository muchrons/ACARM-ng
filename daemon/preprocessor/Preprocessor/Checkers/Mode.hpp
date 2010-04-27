/*
 * Mode.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_MODE_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_MODE_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

namespace Preprocessor
{
namespace Checkers
{

/** \brief base class for all checkers.
 */
class Mode: private boost::noncopyable
{
public:
  /** \brief virtual d-tor to ensure proper destruction.
   */
  virtual ~Mode(void);

  /** \brief check if given string matches predefined condition.
   *  \param str string to check.
   *  \return true if string matches condition, false otherwise.
   */
  virtual bool check(const std::string &str) const = 0;
}; // class Mode

} // namespace Checkers
} // namespace Preprocessor

#endif
