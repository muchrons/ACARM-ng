/*
 * EditableCString.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_EDITABLECSTRING_HPP_FILE
#define INCLUDE_TRIGGER_GG_EDITABLECSTRING_HPP_FILE

#include <string>
#include <boost/scoped_array.hpp>

// TODO: comments

namespace Trigger
{
namespace GG
{

class EditableCString
{
public:
  EditableCString(const char *str);
  EditableCString(const std::string &str);

  const char *get(void) const
  {
    return str_.get();
  }

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
