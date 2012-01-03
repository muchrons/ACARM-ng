#include <iostream>

#include "ConfigIO/Singleton.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Commons/Filesystem/openFile.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Preprocessor/Logic.hpp"
#include "RFCIO/IDMEF/XMLParser.hpp"
#include "RFCIO/XML/Reader.hpp"
#include "Plugins/Loader.hpp"

using namespace std;
using namespace Persistency;
namespace FS=Commons::Filesystem;
using Base::Filesystem::Path;
using Commons::SharedPtrNotNULL;


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
    const Path root(argv[1]);
    if( FS::isFileSane(root)==false )
    {
      cerr<<"file '"<<root<<"' doesn't look sane - aboring"<<endl;
      return 2;
    }

    // load required plugins
    const Path      pluginsDir=ConfigIO::Singleton::get()->generalConfig().getPluginsDir();
    Plugins::Loader loader(pluginsDir);

    // create stub persistency
    const IO::BackendFactory::FactoryTypeName name("stubs");
    const IO::BackendFactory::Options         options=IO::BackendFactory::Options();
    IO::ConnectionPtrNN                       conn( IO::BackendFactory::create(name, options).release() );
    IO::Transaction                           t( conn->createNewTransaction("ignore_persisntent_ops") );

    // open and read input file
    SharedPtrNotNULL<fstream>  file=FS::openFile(root, FS::Mode::READ);
    RFCIO::XML::Reader         reader;
    const xmlpp::Document     &xml=reader.read(*file);
    RFCIO::IDMEF::XMLParser    parser(xml, conn, t);
    AlertPtrNN                 alert=parser.getAlert();
    GraphNodePtrNN             leaf( new GraphNode(alert, MetaAlert::ID(123), conn, t) );
    // just to avoid warnings in log
    t.commit();

    // check if alert is accepted or not
    const ConfigIO::Preprocessor::Config &cfg=ConfigIO::Singleton::get()->preprocessorConfig();
    const Preprocessor::Logic             pproc(cfg);
    const bool                            decision=pproc.checkAccept(leaf);

    // show summary
    cout<<"alert \""<<alert->getName().get()<<"\" is ";
    if(decision)
      cout<<"ACCEPTED";
    else
      cout<<"REJECTED";
    cout<<" by the preprocessor"<<endl;
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
