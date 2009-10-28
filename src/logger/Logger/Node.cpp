/*
 * Node.cpp
 *
 */
#include <sstream>
#include <sys/timeb.h>
#include <iostream> // TODO: remove it                                                                                  

#include "Logger/Node.hpp"
#include "Logger/Formatter.hpp"

using namespace std;


namespace Logger
{

void Node::log(Priority      pri,
               const char   *file,
               const char   *call,
               unsigned int  line,
               const char   *msg) const
{
  // get current time-stamp
  timeb ts;
  ftime(&ts);

  // format string
  stringstream    ss;
  const Formatter fmt=Formatter();
  fmt.format(ss, ts, nn_, pri, file, call, line, msg);

  // TODO: make this real logging
  cout<<ss.str();
}

} // namespace Logger
