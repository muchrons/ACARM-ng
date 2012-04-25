/*
 * Formatter.cpp
 *
 */
#include <cstring>
#include <cstdio>
#include <cctype>
#include <boost/thread.hpp>
#include <cassert>

#include "Logger/Formatter.hpp"

using namespace std;

namespace
{
const char *skipFirstDir(const char *in)
{
  const char *out=in;
  while(*out!=0 && *out!='/')
    ++out;

  // end of string?
  if(*out==0)
    return in;
  // skip '/'
  return out+1;
} // skipFirstDir()

// this f-ction uses little trick, knowing constant paths part in build process...
const char *truncatePath(const char *in)
{
  assert(in!=NULL);
  const char *out;

  // skip header's paths
  out=strstr(in, "/includes/");
  if(out!=NULL)
    return skipFirstDir(out+10);

  // skip sources paths
  out=strstr(in, "/daemon/");
  if(out!=NULL)
    return skipFirstDir(out+8);

  // nothing can be done (?)
  return in;
} // truncatePath()
} // unnamed namespace

namespace Logger
{

void Formatter::format(std::stringstream &ssOut,
                       const timeb       &ts,
                       const NodeName    &nn,
                       Priority           pri,
                       const char        *file,
                       const char        *call,
                       unsigned int       line,
                       const char        *msg)
{
  // get priority as string
  const char *priStr=pri2str(pri);
  assert(priStr!=NULL);

  struct tm *timeinfo = localtime(&ts.time);

  // get zero-padded millisecond part of timestamp
  char tmp[4+1];    // 1000 is max.
  assert(ts.millitm<1000);
  snprintf(tmp, sizeof(tmp), "%03d", ts.millitm);    // foramt zero-padded for readability
  assert( strlen(tmp)<sizeof(tmp) );

  // ensure all strings will have some value:
  file=strFix(file);
  call=strFix(call);
  msg =strFix(msg);
  // make path less verbose
  file=truncatePath(file);

  std::string mon((timeinfo->tm_wday<10?"0":""));

  // format string
  ssOut<<priStr<<"@"
       <<int2Day(timeinfo->tm_wday)<<" "
       <<int2Month(timeinfo->tm_mon)<<" "
       <<timeinfo->tm_mday<<" "
       <<timeinfo->tm_year+1900<<" "
       <<formatTime(timeinfo->tm_hour)<<":"
       <<formatTime(timeinfo->tm_min)<<":"
       <<formatTime(timeinfo->tm_sec)
       <<"."<<tmp<<"/"
       <<nn.get()<<" ["
       <<idMap_.getThreadID()<<"] "
       <<file<<":"
       <<line<<" "
       <<call<<": ";
  // append message to stream, ensuring that special chars will be removed
  appendValidMessage(ssOut, msg);
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

const std::string Formatter::formatTime(const int n) const
{
  std::stringstream ss;
  if(n<10)
    ss<<"0"<<n;
  else
    ss<<n;
  return ss.str();
}

const char *Formatter::int2Day(const int dn) const
{
  switch(dn)
  {
    case 0: return "Sun";
    case 1: return "Mon";
    case 2: return "Tue";
    case 3: return "Wed";
    case 4: return "Thu";
    case 5: return "Fri";
    case 6: return "Sat";
    default:
         assert( !"invalid day number spoted (?!)" );
  }
  // code never reaches here
  return "?";
}

const char *Formatter::int2Month(const int mn) const
{
  switch(mn)
  {
    case 0: return "Jan";
    case 1: return "Feb";
    case 2: return "Mar";
    case 3: return "Apr";
    case 4: return "May";
    case 5: return "Jun";
    case 6: return "Jul";
    case 7: return "Aug";
    case 8: return "Sep";
    case 9: return "Oct";
    case 10: return "Nov";
    case 11: return "Dec";
    default:
         assert( !"invalid month number spoted (?!)" );
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

void Formatter::appendValidMessage(std::stringstream &ssOut, const char *msg) const
{
  assert(msg!=NULL);
  char buf[64];
  int  i=0;
  for(const char *it=msg; *it!=0; ++it)
  {
    // append
    if( *it!='\n' && isprint(*it) && *it!='\t' )
      buf[i]=*it;
    else
      buf[i]='.';
    ++i;
    // buffer is full?
    if( i==sizeof(buf)-1 )
    {
      buf[i]=0;     // null-terminate string
      ssOut<<buf;   // flush
      i=0;          // reset counter
    }
  } // for(msg)
  // final flush on oed of string
  buf[i]=0;         // null-terminate string if not already done
  ssOut<<buf;       // flush
}

} // namespace Logger
