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

/** \brief helper class used for handling exception from python's code and interpreter.
 */
class ExceptionHandle
{
public:
  /** \brief exception thrown when ExceptionHandle is used when no exception has been risen.
   */
  struct NoException: public Exception
  {
    /** \brief create exception's instance.
     *  \param where location where exception has been rised.
     */
    explicit NoException(const Location &where);
  };

  /** \brief gather exception data in a C++ native form.
   *  \note when all is done exception state is cleard in the interpreter with clearState() call.
   */
  ExceptionHandle(void);

  /** \brief rethrown exception with pieces of information gathered from Python.
   *  \note throws ExceptionFromScript.
   */
  void rethrow(void) const;

  /** \brief returns exception's info as text.
   *  \return full exception description and content.
   */
  std::string str(void) const;
  /** \brief gets exception type name.
   */
  const std::string &getType(void) const
  {
    return type_;
  }
  /** \brief gets exception message.
   */
  const std::string &getMessage(void) const
  {
    return msg_;
  }
  /** \brief gets backtrace from where exception has been risen.
   */
  const std::string &getBacktraceStr(void) const
  {
    return backtrace_;
  }

  /** \brief cancel exception state in the interpreter.
   */
  static void clearState(void);

private:
  std::string type_;
  std::string msg_;
  std::string backtrace_;
}; // class ExceptionHandle

} // namespace PythonAPI

#endif
