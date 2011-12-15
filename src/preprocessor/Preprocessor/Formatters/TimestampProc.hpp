/*
 * TimestampProc.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_TIMESTAMPPROC_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_TIMESTAMPPROC_HPP_FILE

#include <string>
#include <ctime>

#include "Preprocessor/Formatters/Base.hpp"
#include "Preprocessor/Formatters/ExceptionNotANumber.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief class created formatted representation of a given timestamp.
 */
class TimestampProc: public Base
{
public:
  struct ExceptionInvalidFormat: public Exception
  {
    ExceptionInvalidFormat(const Location &where, const std::string &what);
  }; // struct ExceptionInvalidFormat

  struct ExceptionConvertionError: public Exception
  {
    ExceptionConvertionError(const Location &where, time_t ts);
  }; // struct ExceptionConvertionError

  /** \brief create object instance of a given action.
   *  \param ts     timestamp to use
   *  \param format textual representation to create from a timestamp.
   */
  TimestampProc(BasePtrNN ts, const std::string &format);

private:
  virtual std::string execImpl(const Arguments &args) const;
  std::string formatTimestamp(time_t ts) const;

  const std::string format_;
}; // class TimestampProc

} // namespace Formatters
} // namespace Preprocessor

#endif
