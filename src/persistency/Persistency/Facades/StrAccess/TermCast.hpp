/*
 * TermCast.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_TERMCAST_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_TERMCAST_HPP_FILE

/* public header */

#include <string>

#include "System/NoInstance.hpp"
#include "Commons/Convert.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

struct TermCast: private System::NoInstance
{
  template<typename T>
  static std::string cast(const T &t)
  {
    return Commons::Convert::to<std::string>(t);
  }

  template<typename T>
  static std::string cast(const T *t)
  {
    if(t==NULL)
      return null();
    return cast(*t);
  }

private:
  static const char *null(void)
  {
    return "<NULL>";
  }
}; // struct TermCast


template<>
inline std::string TermCast::cast<char>(const char *t)
{
  if(t==NULL)
    return null();
  return std::string(t);
}

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
