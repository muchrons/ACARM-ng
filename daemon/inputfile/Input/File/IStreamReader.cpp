/*
 * IStreamReader.cpp
 *
 */
#include <iostream>
#include <unistd.h>
#include <cassert>
#include <boost/thread.hpp>

#include "Input/File/IStreamReader.hpp"

using namespace std;

namespace Input
{
namespace File
{

IStreamReader::IStreamReader(std::istream &input):
  input_(input),
  ss_(new stringstream)
{
}

IStreamReader::Line IStreamReader::readLine(unsigned int timeout)
{
  const time_t deadline=time(NULL)+timeout;
  do
  {
    boost::this_thread::interruption_point();   // for beter responsivness in threads

    // test if we have anything
    if( ss_->str().find("\n")!=string::npos )   // we have line end!
    {
      string str;
      getline(*ss_, str);                       // read whole line to temporary buffer
      truncateStringstreamBy( str.length() );   // free extra resources allocated by stringstream
      return Line(true, str);                   // return results
    }

    // read data in non-blocking manier
    const size_t size=input_.readsome( tmp_, sizeof(tmp_)-1 );
    tmp_[size]=0;
    assert( size<sizeof(tmp_) && "looks like terminating '\\0' is gone" );
    if(size==0)
    {
      // if no data has been read, wait a while
      sleep(1);
    }
    else
    {
      // if has some data, add them to the stream
      assert( string(tmp_).length()<sizeof(tmp_) && "missing terminating '\\0'");
      *ss_<<tmp_;                               // add data to stream
    }

    // check stream after read
    if( !input_.good() )
      throw Exception(SYSTEM_SAVE_LOCATION, "unknown input stream error");
  }
  while( time(NULL)<=deadline );
  // timeout occured - noting new has been read
  return Line(false, "");
}

void IStreamReader::truncateStringstreamBy(const size_t len)
{
  // this block overcomes issue of stringstream - holding data forever...
  boost::scoped_ptr<stringstream> tmp(new stringstream);
  const string &left=ss_->str();
  assert( len<=left.length() );
  *tmp<<left.c_str()+len;
  ss_.swap(tmp);
}

} // namespace File
} // namespace Input
