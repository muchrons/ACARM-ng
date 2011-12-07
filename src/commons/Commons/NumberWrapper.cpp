/*
 * NumberWrapper.cpp
 *
 */
#include <ctype.h>

#include "Commons/Convert.hpp"
#include "Commons/NumberWrapper.hpp"

namespace Commons
{

NumberWrapper::NumberWrapper(const std::string &str):
  isNum_( isNumber(str) )
{
  if( isNumber() )
    value_=Convert::to<double>(str);
}

bool NumberWrapper::isNumber(const std::string &str) const
{
  // empty string is not a number
  if( str.length()==0 )
    return false;

  // check each char
  bool point=false;
  bool sign =false;
  for(std::string::const_iterator it=str.begin(); it!=str.end(); ++it)
  {
    if( isdigit(*it) )
      continue;

    if(*it=='.')    // NOTE: comma is not welcomed here
    {
      if(point)
        return false;
      point=true;
      continue;
    }

    if(*it=='-' || *it=='+')
    {
      if(sign)
        return false;
      sign=true;
      continue;
    }

    // unknown character - return error
    return false;
  } // for(chars)

  // sign, if present, must be the first char
  if(sign && str[0]!='-' && str[0]!='+')
    return false;

  // all tests passed - this is a number
  return true;
}

} // namespace Commons
