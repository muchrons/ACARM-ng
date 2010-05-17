/*
 * EditableCString.cpp
 *
 */
#include <cstring>
#include <cassert>

#include "Trigger/GG/EditableCString.hpp"

namespace Trigger
{
namespace GG
{


EditableCString::EditableCString(const char *str)
{
  makeFrom(str);
}

EditableCString::EditableCString(const std::string &str)
{
  makeFrom( str.c_str() );
}

void EditableCString::makeFrom(const char *str)
{
  // nothing to be done?
  if(str==NULL)
  {
    assert( str_.get()==NULL );
    return;
  }

  // allocate new string
  const size_t len=strlen(str);
  str_.reset(new char[len+1]);
  strcpy(str_.get(), str);
  assert( strlen( str_.get() )==len );      // ensure NULL-termination
  assert( std::string(str)==str_.get() );   // ensure identical content
}

} // namespace GG
} // namespace Trigger
