/*
 * Null.hpp
 *
 */
#ifndef INCLUDE_LOGGER_APPENDERS_NULL_HPP_FILE
#define INCLUDE_LOGGER_APPENDERS_NULL_HPP_FILE

#include <vector>

#include "Logger/Appenders/Base.hpp"


namespace Logger
{
namespace Appenders
{

/** \brief appender ignoring its input.
 */
class Null: public Base
{
public:
  /** \brief static method for getting type name.
   *  \return compile-time pointer to type name.
   */
  static const char *getThisTypeName(void)
  {
    return "Null";
  }

private:
  virtual void appendImpl(const std::string &str);
  virtual const char *getTypeNameImpl(void) const;
}; // class Null

} // namespace Appenders
} // namespace Logger

#endif
