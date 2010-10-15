/*
 * Dumper.cpp
 *
 */
#include <iostream>
#include <cstdio>
#include <cstring>

#include "Commons/Convert.hpp"
#include "Commons/Filesystem/createFile.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Connection.hpp"
#include "RFCIO/IDMEF/XMLCreator.hpp"
#include "PDump/Dumper.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons;
using namespace Commons::Filesystem;
using namespace Persistency;

namespace PDump
{

Dumper::Dumper(std::ostream &out, std::ostream &errOut):
  out_(out),
  errOut_(errOut)
{
}


void Dumper::restoreBetween(const Persistency::Timestamp  from,
                            const Persistency::Timestamp  to,
                            NodesVector                  &nodesOut)
{
  // connect to persistency storage
  out_<<"connecting to persistency storage"<<endl;
  IO::ConnectionPtrNN conn( IO::create() );
  out_<<"connected"<<endl;

  // dump data base content
  out_<<"opening transaction"<<endl;
  IO::Transaction           t( conn->createNewTransaction("persistency_dumper") );
  IO::RestorerAutoPtr       restorer=conn->restorer(t);
  out_<<"restoring nodes between "<<from.str()<<" and "<<to.str()<<endl;
  restorer->restoreBetween(nodesOut, from, to);
  out_<<"restoring's done"<<endl;
  t.commit();
}


std::pair<int, int> Dumper::writeToDir(const NodesVector &nodes, const boost::filesystem::path &outDir)
{
  int writes  =0;
  int attempts=0;

  if( !isDirectorySane(outDir) )
    throw std::runtime_error("output directory is NOT sane");

  out_<<"writing all alerts to files"<<endl;
  for(IO::Restorer::NodesVector::const_iterator it=nodes.begin(); it!=nodes.end(); ++it)
  {
    if( !(*it)->isLeaf() )
      continue;
    try
    {
      ++attempts;
      const MetaAlert::ID::Numeric id=(*it)->getMetaAlert().getID().get();
      char                         percent[3+1+2+1]; // NNN.MM\0
      sprintf(percent, "%3.2f", (100.0*attempts)/nodes.size() );
      assert( strlen(percent)<sizeof(percent) );
      errOut_<<"\rwriting "<<percent<<"\% done (alert ID "<<id<<")";
      RFCIO::IDMEF::XMLCreator     x(**it);
      const path                   file ="idmef_" + Convert::to<string>(id) + ".xml";
      SharedPtrNotNULL<fstream>    fstrm=createFile(outDir/file);
      x.getDocument().write_to_stream(*fstrm, "UTF-8");
      ++writes;
    }
    catch(const RFCIO::Exception &ex)
    {
      errOut_<<"RFC I/O module expcetion: "<<ex.what()<<endl;
      errOut_<<"continuing any way..."<<endl;
    }
    catch(const std::exception &ex)
    {
      errOut_<<"expcetion: "<<ex.what()<<endl;
      errOut_<<"continuing any way..."<<endl;
    }
  } // for(nodes)
  errOut_<<endl;

  // summary and exit
  if(writes==attempts)
    out_<<"wrote "<<writes<<" alerts to disk"<<endl;
  else
    errOut_<<"wrote "<<writes<<" of total "<<attempts<<" alerts to disk - THERE WHERE ERRORS"<<endl;
  return make_pair(writes, attempts);
}

} // namespace PDump
