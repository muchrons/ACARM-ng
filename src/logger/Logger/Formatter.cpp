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

  // get zero-padded millisecond part of timestamp
  char tmp[4+1];    // 1000 is max.
  assert(ts.millitm<1000);
  snprintf(tmp, sizeof(tmp), "%03d", ts.millitm);    // foramt zero-padded for readability
  assert( strlen(tmp)<sizeof(tmp) );

  // convert timestamp to string
  struct tm *timeinfo = localtime(&ts.time);
  char *time = asctime(timeinfo);  // convert timeinfo structure to c-string
  if(time != NULL)
    time[strlen(time) - 1] = '\0';
  // ensure all strings will have some value:
  file=strFix(file);
  call=strFix(call);
  msg =strFix(msg);
  // make path less verbose
  file=truncatePath(file);

  // format string
  ssOut<<priStr<<"@"
       <<time<<"."<<tmp<<"/"
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
