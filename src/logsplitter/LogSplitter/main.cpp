#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "LogSplitter/NumberedFilesSet.hpp"
#include "LogSplitter/Parser.hpp"

using namespace std;
using namespace LogSplitter;


int main(void)
{
  int ret=0;
  try
  {
    // create output buffers collection
    NumberedFilesSet files("thread_", 32*1024*1024);

    // process each and every line
    while(true)
    {
      string line;
      // read single line
      getline(cin, line);
      if(!cin)
        break;
      // parse thread number
      const Parser parser(line);
      // append to the proper file
      NumberedFilesSet::FileStreamPtr out=files.get( parser.get() );
      assert( out.get()!=NULL );
      (*out)<<line<<endl;
    }

    // write remaining data from buffers
    files.flush();
  }
  catch(const std::exception &ex)
  {
    cerr<<"exception: "<<ex.what()<<endl;
    ret=40;
  }
  catch(...)
  {
    cerr<<"unknown expection..."<<endl;
    ret=50;
  }

  return ret;
}
