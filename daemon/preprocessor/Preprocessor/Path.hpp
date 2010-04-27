/*
 * Path.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_PATH_HPP_FILE
#define INCLUDE_PREPROCESSOR_PATH_HPP_FILE

#include <string>
#include <vector>

#include "Preprocessor/ExceptionInvalidPath.hpp"

// TODO: comment

namespace Preprocessor
{

class Path
{
private:
  typedef std::vector<std::string> Elements;

public:
  typedef Elements::const_iterator const_iterator;

  explicit Path(const std::string &path);

  const_iterator begin(void) const
  {
    return e_.begin();
  }

  const_iterator end(void) const
  {
    return e_.end();
  }

  const std::string get(void) const
  {
    return path_;
  }

private:
  void tokenizePath(void);

  std::string path_;
  Elements    e_;
}; // class Path

} // namespace Preprocessor

#endif
