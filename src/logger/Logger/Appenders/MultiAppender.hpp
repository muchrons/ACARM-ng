/*
 * MultiAppender.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_MULTIAPPENDER_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_MULTIAPPENDER_HPP_FILE

#include <vector>

#include "Logger/Appenders/Base.hpp"
#include "Logger/ExceptionNullParameter.hpp"

// TODO: comment

namespace Logger
{
namespace Appenders
{

/** \brief multi appender is an appender that aggregates other appenders
 *         making them behave as regular appender. it is used to log to
 *         multiple outputs.
 */
class MultiAppender: public Base
{
public:
  /** \brief collection of appenders to register.
   */
  typedef std::vector<BasePtr> AppVec;

  /** \brief creates MultiAppender with appenders given as a parameter
   *         registered to log to.
   *  \param apps appenders to register.
   */
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
