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
#include "RFCIO/XML/Writer.hpp"
#include "RFCIO/IDMEF/XMLCreator.hpp"
#include "PDump/Dumper.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Commons;
using namespace Commons::Filesystem;
using namespace Persistency;

namespace PDump
{

Dumper::Stats::Stats(size_t total, size_t attempts, size_t writes):
  total_(total),
  writes_(writes),
  attempts_(attempts)
{
  if( getTotal()<getAttempts() )
    throw std::logic_error("total count is less than attempts to write count");
  if( getAttempts()<getWrites() )
    throw std::logic_error("attempts count is less than write count");
}

size_t Dumper::Stats::getTotal(void) const
{
  return total_;
}

size_t Dumper::Stats::getWrites(void) const
{
  return writes_;
}

size_t Dumper::Stats::getAttempts(void) const
{
  return attempts_;
}


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
  out_<<"(note: depending on alerts count this may take some time...)"<<endl;
  restorer->restoreBetween(nodesOut, from, to);
  out_<<"restoring's done"<<endl;
  t.commit();
}


Dumper::Stats Dumper::writeToDir(const NodesVector &nodes, const boost::filesystem::path &outDir)
{
  int writes  =0;
  int attempts=0;
  int count   =0;

  if( !isDirectorySane(outDir) )
    throw std::runtime_error("output directory is NOT sane");

  out_<<"writing all alerts to files"<<endl;
  for(IO::Restorer::NodesVector::const_iterator it=nodes.begin(); it!=nodes.end(); ++it)
  {
    ++count;
    if( !(*it)->isLeaf() )
      continue;
    try
    {
      ++attempts;
      const MetaAlert::ID::Numeric id=(*it)->getMetaAlert()->getID().get();
      char                         percent[3+1+2+1]; // NNN.MM\0
      sprintf(percent, "%3.2f", (100.0*count)/nodes.size() );
      assert( strlen(percent)<sizeof(percent) );
      errOut_<<"\rwriting "<<percent<<"\% done (alert ID "<<id<<")";
      RFCIO::IDMEF::XMLCreator     x(**it);
      const path                   file ="idmef_" + Convert::to<string>(id) + ".xml";
      SharedPtrNotNULL<fstream>    fstrm=createFile(outDir/file);
      RFCIO::XML::Writer           writer( x.getDocument() );
      writer.write(*fstrm);
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

  out_<<count-attempts<<" non-alerts skipped"<<endl;

  // summary and exit
  if(writes==attempts)
    out_<<"wrote "<<writes<<" alerts to disk"<<endl;
  else
    errOut_<<"wrote "<<writes<<" of total "<<attempts<<" alerts to disk - THERE WHERE ERRORS"<<endl;
  return Stats(count, attempts, writes);
}

} // namespace PDump
