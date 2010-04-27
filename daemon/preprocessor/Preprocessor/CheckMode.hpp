/*
 * CheckMode.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_CHECKMODE_HPP_FILE
#define INCLUDE_PREPROCESSOR_CHECKMODE_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>


// TODO: comment

namespace Preprocessor
{

class CheckMode: private boost::noncopyable
{
public:
  virtual ~CheckMode(void);
  virtual bool check(const std::string &str) const = 0;
}; // class CheckMode

} // namespace Preprocessor

#endif
