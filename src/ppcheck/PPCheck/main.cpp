#include <iostream>

#include "Commons/Filesystem/isFileSane.hpp"
#include "Preprocessor/Logic.hpp"
#include "RFCIO/IDMEF/XMLParser.hpp"

using namespace std;


int main(int argc, const char **argv)
{
  // check input
  if(argc!=1+1)
  {
    cerr<<argv[0]<<" <idmef.xml>"<<endl;
    return 1;
  }

  // main applicaiton's part
  int ret=0;
  try
  {
    // get input paramters
    const boost::filesystem::path root(argv[1]);
    if( Filesystem::isFileSane(root)==false )
    {
      cerr<<"file '"<<root<<"' doesn't look sane - aboring"<<endl;
      return 2;
    }

    // check if alert is accepted or not
    cout<<"TODO"<<endl;
  }
  catch(const Preprocessor::Exception &ex)
  {
    cerr<<"preprocessor expcetion: "<<ex.what()<<endl;
    ret=20;
  }
  catch(const RFCIO::Exception &ex)
  {
    cerr<<"RFCIO expcetion: "<<ex.what()<<endl;
    ret=30;
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
