#include <iostream>

#include "Commons/Convert.hpp"
#include "Commons/Filesystem/createFile.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Connection.hpp"
#include "RFCIO/IDMEF/XMLCreator.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons;
using namespace Commons::Filesystem;
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
  int    ret     =0;
  size_t attempts=0;
  size_t writes  =0;
  try
  {
    // get input paramters
    const path      root(argv[1]);
    cout<<"dumping alerts to output directory "<<root<<endl;
    if( isDirectorySane(root)==false )
    {
      cerr<<"directory '"<<root<<"' doesn't look sane - aboring"<<endl;
      return 2;
    }
    const Timestamp from( Convert::to<time_t>(argv[2]) );
    cout<<"dumping alerts starting from "<<from.get()<<" ("<<from.str()<<")"<<endl;
    const Timestamp to( Convert::to<time_t>(argv[3]) );
    cout<<"dumping alerts until "<<to.get()<<" ("<<to.str()<<")"<<endl;

    // connect to persistency storage
    cout<<"connecting to persistency storage"<<endl;
    IO::ConnectionPtrNN conn( IO::create() );
    cout<<"connected"<<endl;

    // dump data base content
    cout<<"opening transaction"<<endl;
    IO::Transaction           t( conn->createNewTransaction("persistency_dumper") );
    IO::RestorerAutoPtr       restorer=conn->restorer(t);
    IO::Restorer::NodesVector out;
    cout<<"restoring nodes between "<<from.str()<<" and "<<to.str()<<endl;
    restorer->restoreBetween(out, from, to);
    cout<<"restoring's done"<<cout;
    t.commit();

    // save results to files
    cout<<"writing all alerts to files"<<endl;
    for(IO::Restorer::NodesVector::const_iterator it=out.begin(); it!=out.end(); ++it)
      if( (*it)->isLeaf() )
      {
        try
        {
          ++attempts;
          const MetaAlert::ID::Numeric id=(*it)->getMetaAlert().getID().get();
          cerr<<"\rwriting alert ID "<<id<<" / "<<(100.0*attempts)/out.size()<<"%% done...";
          RFCIO::IDMEF::XMLCreator     x(**it);
          const path                   file ="idmef_" + Convert::to<string>(id) + ".xml";
          SharedPtrNotNULL<fstream>    fstrm=createFile(root/file);
          x.getDocument().write_to_stream(*fstrm);
          ++writes;
        }
        catch(const RFCIO::Exception &ex)
        {
          cerr<<"RFC I/O module expcetion: "<<ex.what()<<endl;
          ret=10;
        }
        catch(const std::exception &ex)
        {
          cerr<<"expcetion: "<<ex.what()<<endl;
          ret=20;
        }
      }

    // all ok
    ret=0;
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

  // summary and exit
  if(writes==attempts)
    cout<<"wrote "<<writes<<" alerts to disk"<<endl;
  else
    cerr<<"wrote "<<writes<<" of total "<<attempts<<" alerts to disk - THERE WHERE ERRORS"<<endl;
  cout<<"returning with error code "<<ret<<endl;
  return ret;
}
