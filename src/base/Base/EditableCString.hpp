/*
 * EditableCString.hpp
 *
 */
#ifndef INCLUDE_BASE_EDITABLECSTRING_HPP_FILE
#define INCLUDE_BASE_EDITABLECSTRING_HPP_FILE

/* public header */

#include <string>
#include <boost/scoped_array.hpp>

namespace Base
{

/** \brief holder that makes a copy of input string in local buffer.
 *
 *  saved string can be edited by other calls. note that size of internal
 *  buffer never grows. use with care.
 *  this can be used to fix const-INcorrect code without const-casts or
 *  to cooperate with some C libraries that changes their paramter.
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
   *  \return pointer to string.
   */
  const char *get(void) const
  {
    return str_.get();
  }
  /** \brief gets internal data (via pointer) - non-const version.
   *  \return pointer to string.
   */
  char *get(void)
  {
    return str_.get();
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

  boost::scoped_array<char> str_;
}; // class LoginParameters

} // namespace Base


namespace std
{
/** \brief common call for swapping content of two elements of EditableCString type.
 *  \param str1 first elemtns of the operation.
 *  \param str2 second element of the operation.
 */
inline void swap(Base::EditableCString &str1, Base::EditableCString &str2)
{
  str1.swap(str2);
} // swap()
} // namespace std

#endif
