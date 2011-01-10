#include <iostream>

#include "Commons/Convert.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"
#include "PDump/Dumper.hpp"

using namespace std;
using namespace PDump;
using namespace Commons;
using namespace Persistency;

int main(int argc, const char **argv)
{
  // check input
  if(argc!=1+3)
  {
    cerr<<argv[0]<<" <out_dir> <from_timestamp> <to_timestamp>"<<endl;
    return 1;
  }

  // start procedure
  int ret=0;
  try
  {
    // get input paramters
    const boost::filesystem::path root(argv[1]);
    cout<<"dumping alerts to output directory "<<root<<endl;
    if( Filesystem::isDirectorySane(root)==false )
    {
      cerr<<"directory '"<<root<<"' doesn't look sane - aboring"<<endl;
      return 2;
    }
    const Timestamp from( Convert::to<time_t>(argv[2]) );
    cout<<"dumping alerts starting from "<<from.get()<<" ("<<from.str()<<")"<<endl;
    const Timestamp to( Convert::to<time_t>(argv[3]) );
    cout<<"dumping alerts until "<<to.get()<<" ("<<to.str()<<")"<<endl;

    // perform dumping itself
    Dumper::Stats stats(0,0,0);
    {
      Dumper              d(cout, cerr);
      Dumper::NodesVector nodes;
      d.restoreBetween(from, to, nodes);
      stats=d.writeToDir(nodes, root);
    }

    // output some stats
    cout<<"----- summary -----"<<endl;
    const int writes  =stats.getWrites();
    const int attempts=stats.getAttempts();
    if(writes==attempts)
      cout<<"wrote "<<writes<<" alerts to disk"<<endl;
    else
    {
      cerr<<"wrote "<<writes<<" of total "<<attempts<<" alerts to disk - THERE WHERE ERRORS"<<endl;
      ret=10;
      cout<<"returning with error code "<<ret<<endl;
    }
  }
  catch(const Persistency::Exception &ex)
  {
    cerr<<"persistency expcetion: "<<ex.what()<<endl;
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
