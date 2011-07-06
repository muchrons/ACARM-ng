/*
 * Parser.cpp
 *
 */
#include <string>
#include <boost/lexical_cast.hpp>

#include "LogSplitter/Parser.hpp"

using namespace std;


namespace LogSplitter
{

namespace
{
// look for sequence " [<digits>] " and return it
inline string parseSequence(const string &str)
{
  for(string::const_iterator it=str.begin(); it!=str.end(); ++it)
  {
    if(*it!=' ')
      continue;
    ++it;
    if( it==str.end() )
      break;

    if(*it!='[')
      continue;
    ++it;
    if( it==str.end() )
      break;

    if( !isdigit(*it) )
      continue;
    // save position of the first character
    const string::const_iterator from=it;

    // skip all digits
    do
    {
      ++it;
      if( it==str.end() )
        break;
    }
    while( isdigit(*it) );

    if( it==str.end() )
      break;
    if(*it!=']')
      continue;

    // save posiotn of the ending
    const string::const_iterator to=it;

    ++it;
    if( it==str.end() )
      break;

    if(*it!=' ')
      break;

    // if we're here, it means that parsing succeeded
    return string(from, to);
  }

  throw runtime_error("unable to parse input line: " + str);
} // parseSequence()

inline unsigned int toNumber(const string &num)
{
  return boost::lexical_cast<unsigned int>(num);
} // toNumber()
} // unnamed namespace


Parser::Parser(const std::string &str):
  num_( toNumber( parseSequence(str) ) )
{
}

} // namespace LogSplitter
