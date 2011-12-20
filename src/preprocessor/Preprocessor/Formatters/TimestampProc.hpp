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
  /** \brief exception throw on invalid format detection.
   */
  struct ExceptionInvalidFormat: public Exception
  {
    /** \brief create error message instance.
     *  \param where location of error detection.
     *  \param what  error details.
     */
    ExceptionInvalidFormat(const Location &where, const std::string &what);
  }; // struct ExceptionInvalidFormat

  /** \brief exception thrown on invalid convertion from timestamp.
   */
  struct ExceptionConvertionError: public Exception
  {
    /** \brief creates error message instance.
     *  \param where location of error decetion.
     *  \param ts    timestamp who's convertion failed.
     */
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
