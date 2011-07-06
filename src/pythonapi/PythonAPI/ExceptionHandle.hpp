/*
 * ExceptionHandle.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_EXCEPTIONHANDLE_HPP_FILE
#define INCLUDE_PYTHONAPI_EXCEPTIONHANDLE_HPP_FILE

/* public header */

#include <string>

#include "PythonAPI/Exception.hpp"
#include "PythonAPI/ExceptionFromScript.hpp"


namespace PythonAPI
{

class ExceptionHandle
{
public:
  struct NoException: public Exception
  {
    explicit NoException(const Location &where);
  };

  ExceptionHandle(void);

  void rethrow(void) const;

  std::string str(void) const;

  const std::string &getType(void) const
  {
    return type_;
  }
  const std::string &getMessage(void) const
  {
    return msg_;
  }
  const std::string &getBacktraceStr(void) const
  {
    return backtrace_;
  }

  static void clearState(void);

private:
  std::string type_;
  std::string msg_;
  std::string backtrace_;
}; // class ExceptionHandle

} // namespace PythonAPI

#endif
