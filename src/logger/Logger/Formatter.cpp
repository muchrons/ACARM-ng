/*
 * Formatter.cpp
 *
 */
#include <cstring>
#include <cstdio>
#include <cassert>

#include "Logger/Formatter.hpp"

using namespace std;


namespace Logger
{

void Formatter::format(std::stringstream &ssOut,
                       const timeb       &ts,
                       const NodeName    &nn,
                       Priority           pri,
                       const char        *file,
                       const char        *call,
                       unsigned int       line,
                       const char        *msg) const
{
  // get priority as string
  const char *priStr=pri2str(pri);
  assert(priStr!=NULL);

  // get zero-padded millisecond part of timestamp
  char tmp[4+1];    // 1000 is max.
  assert(ts.millitm<1000);
  snprintf(tmp, sizeof(tmp), "%03d", ts.millitm);    // foramt zero-padded for readability
  assert( strlen(tmp)<sizeof(tmp) );

  // ensure all strings will have some value:
  file=strFix(file);
  call=strFix(call);
  msg =strFix(msg);

  // format string
  ssOut<<priStr<<"@"
       <<ts.time<<"."<<tmp<<"/"
       <<nn.get()<<" "
       <<file<<":"
       <<call<<":"
       <<line<<": "
       <<msg;
}


void Formatter::swap(Formatter &)
{
  // note: implementation is empty, since format does not have any
  //       fields as for now.
}


const char *Formatter::pri2str(Priority pri) const
{
  switch( pri.toInt() )
  {
    case Priority::DEBUG: return "DEBUG";
    case Priority::INFO:  return "INFO ";
    case Priority::WARN:  return "WARN ";
    case Priority::ERROR: return "ERROR";
    case Priority::FATAL: return "FATAL";
    default:
         assert( !"invalid priority spoted (?!)" );
  }
  // code never reaches here
  return "?";
}

const char *Formatter::strFix(const char *str) const
{
  if(str==NULL)
    return "NULL";
  return str;
}

} // namespace Logger
