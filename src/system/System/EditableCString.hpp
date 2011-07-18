/*
 * EditableCString.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EDITABLECSTRING_HPP_FILE
#define INCLUDE_SYSTEM_EDITABLECSTRING_HPP_FILE

/* public header */

#include <string>
#include <cstring>
#include <boost/scoped_array.hpp>

namespace System
{

/** \brief holder that makes a copy of input string in local buffer.
 *
 *  saved string can be edited by other calls. note that size of internal
 *  buffer never grows. use with care.
 *  this can be used to fix const-INcorrect code without const-casts or
 *  to cooperate with some C libraries that changes their paramters unexpectedly.
 *  consider this as a helper-code, that good design should probably never use.
 */
class EditableCString
{
public:
  /** \brief create object from c-string.
   *  \param str string to be copyied.
   *  \note c-tor is not 'explicit' on purpose, to make usage more intiutive.
   */
  EditableCString(const char *str);
  /** \brief create object from C++ string.
   *  \param str string to be copyied.
   *  \note c-tor is not 'explicit' on purpose, to make usage more intiutive.
   */
  EditableCString(const std::string &str);

  /** \brief gets internal data (via pointer) - const version.
   *  \return pointer to string - may be NULL.
   */
  const char *get(void) const
  {
    return str_.get();
  }
  /** \brief gets internal data (via pointer) - non-const version.
   *  \return pointer to string - may be NULL.
   */
  char *get(void)
  {
    return str_.get();
  }

  /** \brief gives access to a specific char in a string.
   *  \param pos position to read char from
   *  \return reference to the char (can be changed).
   *  \note range checks are NOT performed by this call.
   */
  char &operator[](const size_t pos)
  {
    assert(get()!=NULL && "NULL pointer dereference");
    assert(pos<length() && "index out of bound");
    return str_[pos];
  }
  /** \brief gives read-only access to a specific char in a string.
   *  \param pos position to read char from
   *  \return char's value.
   *  \note range checks are NOT performed by this call.
   */
  char operator[](const size_t pos) const
  {
    assert(get()!=NULL && "NULL pointer dereference");
    assert(pos<length() && "index out of bound");
    return str_[pos];
  }

  /** \brief swap contents of two strings.
   *  \param other object ot swap content with.
   */
  void swap(EditableCString &other)
  {
    str_.swap(other.str_);
  }

private:
  void makeFrom(const char *str);
  size_t length(void) const;

  boost::scoped_array<char> str_;
}; // class EditableCString

} // namespace System


namespace std
{
/** \brief common call for swapping content of two elements of EditableCString type.
 *  \param str1 first elemtns of the operation.
 *  \param str2 second element of the operation.
 */
inline void swap(System::EditableCString &str1, System::EditableCString &str2)
{
  str1.swap(str2);
} // swap()
} // namespace std

#endif
