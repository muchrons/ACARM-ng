/*
 * Base.hpp
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONS_BASE_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONS_BASE_HPP_FILE

/* public header */

#include <string>
#include <stdexcept>

#include "System/Exceptions/CommonElements.hpp"

namespace System
{
namespace Exceptions
{

/** \brief generic base for exceptions implementation.
 *  \param CRTP    derived class (to start new hierarchy).
 *  \param StdBase base class for exception from sntadard library.
 */
template<typename CRTP, typename StdBase>
class Base: public    StdBase,
            protected CommonElements
{
public:
  /** \brief destructor requires special flags.
   */
  virtual ~Base(void) throw()
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
  /** \brief gets type info.
   *  \return name of real type for this class.
   */
  std::string getTypeName(void) const
  {
    return CommonElements::getTypeName(*this);
  }

protected:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit Base(const T &msg):
    msg_(msg)
  {
  }

  /** \brief create execption with given message.
   *  \param where location where message has been generated.
   *  \param msg   message to represent.
   *  \note use SYSTEM_SAVE_LOCATION macro to generate 'where' field.
   */
  template<typename T>
  explicit Base(const Location &where, const T &msg):
    msg_( cc(where.getStr(), ": ", msg) )
  {
  }

private:
  std::string msg_;
}; // class Base

} // namespace Exceptions
} // namespace System

#endif
