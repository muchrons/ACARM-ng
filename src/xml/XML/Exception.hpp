/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_XML_EXCEPTION_HPP_FILE
#define INCLUDE_XML_EXCEPTION_HPP_FILE

/* public header */

#include <string>
#include <exception>


namespace XML
{

/** \brief internal exception class.
 */
class Exception: public std::exception
{
public:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  Exception(const T &msg):
                msg_(msg)
  {
  }
  /** \brief destructor requires special flags.
   */
  virtual ~Exception(void) throw()
  {
  }

  /** \brief return error message as string.
   *  \return error message.
   */
  inline const std::string& whatAsStr(void) const
  {
    return msg_;
  }
  /** \brief return error message pointer, as const char*.
   *  \return pointer to message.
   */
  virtual const char *what(void) const throw()
  {
    return msg_.c_str();
  }

private:
  std::string msg_;
}; // class Exception

}; // namespace XML

#endif

