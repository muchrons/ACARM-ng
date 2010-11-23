/*
 * IStreamReader.cpp
 *
 */
#include <boost/thread.hpp>
#include <iostream>
#include <unistd.h>
#include <cassert>

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
  for(;;)
  {
    // check stream before read
    if( !input_.good() )
      throw Exception(SYSTEM_SAVE_LOCATION, "unknown input stream error");

    // try reading data until something's found
    size_t size;
    do
    {
      boost::this_thread::interruption_point();

      // before reading any data, test if we have something already in buffer
      if( ss_->str().find("\n")!=string::npos )   // we have line end!
      {
        string str;
        getline(*ss_, str);                       // read whole line to temporary buffer
        truncateStringstreamBy( str.length() );   // free extra resources allocated by stringstream
        return Line(true, str);                   // return results
      }

      // if we do not have enought data, read some in non-blocking manier
      size=input_.readsome( tmp_, sizeof(tmp_)-1 );
      tmp_[size]=0;
      assert( size<sizeof(tmp_) && "looks like terminating '\\0' is gone" );
      if(size>0)
      {
        // if has some data, add them to the stream
        assert( string(tmp_).length()<sizeof(tmp_) && "missing terminating '\\0'");
        *ss_<<tmp_;                               // add data to stream
      }
    }
    while(size!=0);

    // deadline occured?
    if( time(NULL)>=deadline )
      break;

    // if no data has been read and there is still no new line, wait a while
    boost::this_thread::sleep( boost::posix_time::seconds(1) );
  } // for(;;)

  // timeout occured - noting new has been read
  return Line(false, "");
}

void IStreamReader::truncateStringstreamBy(const size_t len)
{
  // this block overcomes issue of stringstream - holding data forever...
  boost::scoped_ptr<stringstream> tmp(new stringstream);
  const string &left=ss_->str();
  assert( len<=left.length() );
  *tmp<<left.c_str()+len+1; // skip along with terminating '\0'
  ss_.swap(tmp);
}

} // namespace File
} // namespace Input
