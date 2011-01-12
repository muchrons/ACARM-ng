/*
 * DShieldParser.cpp
 *
 */
#include <ctype.h>
#include "Filter/IPBlackList/DShieldParser.hpp"

using namespace std;


namespace Filter
{
namespace IPBlackList
{

namespace
{

bool isEOL(const char c)
{
  return (c=='\r' || c=='\n');
}

const char *skipEOLs(const char *in)
{
  assert(in!=NULL);
  while( isEOL(*in) )
    ++in;
  return in;
}

const char *skipLine(const char *in)
{
  assert(in!=NULL);
  // ok - skip until end of the line
  while( *in!=0 && !isEOL(*in) )
    ++in;
  // as the last part skip new lines
  in=skipEOLs(in);
  return in;
}

const char *skipComments(const char *in)
{
  assert(in!=NULL);
  while(*in=='#')
    in=skipLine(in);
  return in;
} // skipComments()

const size_t ipLength=4*3 + 3;

bool isLongEnoughForIP(const char *in)
{
  for(size_t i=0; i<ipLength; ++i)
    if(in[i]==0)    // already the end?
      return false;
  return true;
}

} // unnamed namespace


DShieldParser::const_iterator::const_iterator(const char *str):
  str_(str)
{
  if(str_!=NULL)
  {
    str_=skipComments(str_);
    assert(str_!=NULL);
    // end of unit?
    if(*str_==0)
      str_=NULL;
  }
}

const DShieldParser::const_iterator &DShieldParser::const_iterator::operator++(void)
{
  if(str_==NULL)
    return *this;
  assert(str_!=NULL);

  str_=skipLine(str_);      // this line is not interesting any more
  str_=skipComments(str_);  // skip comments as well
  // if end of stream, mark this as the end
  if(*str_==0)
    str_=NULL;
  return *this;
}

bool DShieldParser::const_iterator::operator==(const const_iterator &other) const
{
  return str_==other.str_;
}

IP DShieldParser::const_iterator::operator*(void) const
{
  assert(str_!=NULL);
  if( !isLongEnoughForIP(str_) || !isblank(str_[ipLength]) )
    throw ExceptionInvalidFormat(SYSTEM_SAVE_LOCATION, str_, "invalid line");

  // construct proper string
  bool        lastDot=true;
  const char *in     =str_;
  char        tmp[ipLength+1]={0};
  for(size_t i=0; i<ipLength && !isblank(*in); ++i)
  {
    if(lastDot)
    {
      lastDot=false;
      if(*in=='0')
      {
        ++in;
        if(*in=='0')
          ++in;
      }
    }
    else
      if(*in=='.')
        lastDot=true;
    tmp[i]=*in;
    ++in;
  }
  assert(tmp[ipLength]==0);
  // build IP from it.
  try
  {
    return IP::from_string(tmp);
  }
  catch(const std::exception &ex)
  {
    string err("unable to parse IP: '");
    err+=tmp;
    err+="' - ";
    err+=ex.what();
    throw ExceptionInvalidFormat(SYSTEM_SAVE_LOCATION, str_, err.c_str() );
  }
  assert(!"code should never reach here");
}


DShieldParser::DShieldParser(const std::string &data):
  data_(data)
{
}

DShieldParser::const_iterator DShieldParser::begin(void) const
{
  return const_iterator( data_.c_str() );
}

DShieldParser::const_iterator DShieldParser::end(void) const
{
  return const_iterator(NULL);
}

} // namespace IPBlackList
} // namespace Filter
