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
  /** \brief filesystem path. */
  typedef boost::filesystem::path Path;

  /** \brief create configuration object.
   *  \param timeout ammount of time to hold meta-alert for.
   *  \param path    path to python script creating object's instance.
   */
  Config(unsigned int timeout, const Path &path):
    timeout_(timeout),
    path_(path)
  {
  }

  /** \brief gets timeout value.
   */
  unsigned int getTimeout(void) const
  {
    return timeout_;
  }

  /** \brief gets path to a Python script.
   */
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
