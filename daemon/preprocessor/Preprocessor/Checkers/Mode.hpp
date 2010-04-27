/*
 * Mode.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKERS_MODE_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKERS_MODE_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>


// TODO: comment

namespace Preprocessor
{
namespace Checkers
{

class Mode: private boost::noncopyable
{
public:
  virtual ~Mode(void);
  virtual bool check(const std::string &str) const = 0;
}; // class Mode

} // namespace Checkers
} // namespace Preprocessor

#endif
