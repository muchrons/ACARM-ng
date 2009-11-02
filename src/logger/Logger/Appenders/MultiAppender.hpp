/*
 * MultiAppender.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_MULTIAPPENDER_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_MULTIAPPENDER_HPP_FILE

#include <vector>

#include "Logger/Appenders/Base.hpp"

// TODO: comment
// TODO: test

namespace Logger
{
namespace Appenders
{

class MultiAppender: public Base
{
public:
  typedef std::vector<BasePtr> AppVec;

  explicit MultiAppender(const AppVec &apps);

  /** \brief static method for getting type name.
   *  \return compile-time pointer to type name.
   */
  static const char *getThisTypeName(void)
  {
    return "MultiAppender";
  }

private:
  virtual void appendImpl(const std::string &str);
  virtual const char *getTypeNameImpl(void) const;

  const AppVec apps_;
}; // class MultiAppender

} // namespace Appenders
} // namespace Logger

#endif
