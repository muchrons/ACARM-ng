/*
 * EditableCString.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_EDITABLECSTRING_HPP_FILE
#define INCLUDE_TRIGGER_GG_EDITABLECSTRING_HPP_FILE

#include <string>
#include <boost/scoped_array.hpp>

namespace Trigger
{
namespace GG
{

/** \brief holder that makes a copy of input string in local buffer.
 *
 *  saved string can be edited by other calls. it is required, since
 *  libgadu does not have consts in few places.
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

private:
  void makeFrom(const char *str);

  boost::scoped_array<char> str_;
}; // class LoginParameters

} // namespace GG
} // namespace Trigger

#endif
