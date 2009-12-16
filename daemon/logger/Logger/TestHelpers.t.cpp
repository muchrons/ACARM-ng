/*
 * TestHelpers.t.cpp
 *
 */
#include <tut.h>
#include <fstream>
#include <cstring>
#include <cassert>

#include "Logger/TestHelpers.t.hpp"

using namespace std;

namespace Logger
{

string getLastLogMessage(void)
{
  ifstream is("acarm-ng.log");
  assert( is.is_open() );
  string out;
  string prev;
  while( !is.eof() )
  {
    prev=out;
    getline(is, out);
  }
  return prev;
} // getLastLogMessage()


void ensureLoggedPart(const char *part)
{
  assert(part!=NULL);
  const string msg=getLastLogMessage();
  if( strstr( msg.c_str(), part)==NULL )
  {
    const string err="invalid message log: '" + msg + "' (part not found: '" + part + "')";
    tut::fail( err.c_str() );
  }
} // ensureLoggedPart()

} // namespace Logger
