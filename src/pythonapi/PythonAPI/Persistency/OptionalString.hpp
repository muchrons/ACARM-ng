/*
 * OptionalString.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_OPTIONALSTRING_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_OPTIONALSTRING_HPP_FILE

/* public header */

#include <string>

#include "System/EditableCString.hpp"


namespace PythonAPI
{
namespace Persistency
{

/** \brief string that can be NULL implementation
 *
 *  there is a need for some sort of copyable implementaiton that can
 *  handle NULLs so that talking with Python's interpretter cen be acchieved.
 */
class OptionalString
{
public:
  /** \brief NULL string by default.
   */
  OptionalString(void):
    str_(NULL)
  {
  }

  /** \brief create object from a given string
   *  \param str string to create from.
   */
  explicit OptionalString(const char *str):
    str_(str)
  {
  }

  /** \brief create object from a given string
   *  \param str string to create from.
   */
  explicit OptionalString(const std::string &str):
    str_(str.c_str())
  {
  }

  /** \brief make object copyable.
   *  \param other object to copy content from.
   */
  OptionalString(const OptionalString &other):
    str_(other.get())
  {
  }

  /** \brief make object copyable by assignment.
   *  \param other object to copy from.
   *  \return reference to this.
   */
  const OptionalString &operator=(const OptionalString &other)
  {
    if(this!=&other)
    {
      OptionalString tmp(other);
      tmp.swap(*this);
    }
    return *this;
  }

  /** \brief gets string value.
   *  \return string value (can be NULL).
   */
  const char *get(void) const
  {
    return str_.get();
  }

  /** \brief swaps the content of thwo objects.
   *  \param other object to swap content with.
   */
  void swap(OptionalString &other)
  {
    str_.swap(other.str_);
  }

private:
  System::EditableCString str_;
}; // class OptionalString

} // namespace Persistency
} // namespace PythonAPI

#endif
