/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_CONFIG_HPP_FILE
#define INCLUDE_FILTER_PYTHON_CONFIG_HPP_FILE

#include <boost/filesystem.hpp>

namespace Filter
{
namespace Python
{

/** \brief configuration class for python filter.
 */
class Config
{
public:
  typedef boost::filesystem::path Path;

  Config(unsigned int timeout, const Path &path):
    timeout_(timeout),
    path_(path)
  {
  }

  unsigned int getTimeout(void) const
  {
    return timeout_;
  }

  const Path &getScriptPath(void) const
  {
    return path_;
  }

private:
  unsigned int timeout_;
  Path         path_;
}; // class Config

} // namespace Python
} // namespace Filter

#endif
