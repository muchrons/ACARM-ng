/*
 * Strategy.cpp
 *
 */
#include <vector>
#include <boost/filesystem.hpp>
#include <cassert>

#include "Commons/Convert.hpp"
#include "Commons/Filesystem/createTempDirectory.hpp"
#include "Commons/Filesystem/createFile.hpp"
#include "RFCIO/XML/Writer.hpp"
#include "RFCIO/IDMEF/XMLCreator.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/countCorrelatedAlerts.hpp"
#include "Algo/computeCertainty.hpp"
#include "Algo/computeSeverity.hpp"
#include "Trigger/ExtApp/Strategy.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Base::Threads;
using namespace Commons;
using namespace Commons::Filesystem;
using namespace Core::Types::Proc;

namespace Trigger
{
namespace ExtApp
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg):
  Trigger::Simple::Strategy( TypeName("extapp"), name, cfg.getThresholdConfig() ),
  cfg_(cfg)
{
}


namespace
{

/** \brief holds ownership of given filesystem elements.
 *
 *  element is automatically removed removed in d-tor.
 */
struct ElementHolder: private boost::noncopyable
{
  explicit ElementHolder(const Logger::Node &log):
    log_(log)
  {
  }

  ~ElementHolder(void)
  {
    for(vector<path>::const_reverse_iterator it=paths_.rbegin(); it!=paths_.rend(); ++it)
    {
      try
      {
        LOGMSG_DEBUG_S(log_)<<"removing element: "<<*it;
        remove(*it);
      }
      catch(...)
      {
        LOGMSG_ERROR_S(log_)<<"error while removing element: "<<*it;
        // nothing can be done here
      }
    }
  }

  void push_back(const path &p)
  {
    paths_.push_back(p);
  }

private:
  Logger::Node log_;
  vector<path> paths_;
}; // struct ElementHolder


/** \brief helper object that writes all leafs to IDMEF.
 */
struct FunctorIDMEF
{
  FunctorIDMEF(const Logger::Node &log, ElementHolder &holder, const path &root):
    log_(log),
    holder_(&holder),
    root_(root)
  {
  }

  void operator()(Persistency::ConstGraphNodePtrNN leaf)
  {
    assert( leaf->isLeaf() && "wrong algorithm selected?" );
    RFCIO::IDMEF::XMLCreator  c(*leaf);
    const string              file="idmef_" + Convert::to<string>( leaf->getMetaAlert()->getID().get() ) + ".xml";
    const path                out =root_ / file;
    SharedPtrNotNULL<fstream> strm=createFile(out);     // create file in the secure way
    holder_->push_back(out);                            // add file for removal
    RFCIO::XML::Writer        writer( c.getDocument() );// XML writer object
    writer.write(*strm);                                // write XML to that file
  }

private:
  Logger::Node   log_;
  ElementHolder *holder_;
  path           root_;
}; // struct FunctorIDMEF

} // unnamed namespace


void Strategy::triggerImpl(const ConstNode &n)
{
  // prepare basic info
  const size_t count    =Algo::countCorrelatedAlerts(n);
  const double priority =Algo::computeSeverity(n);
  const double certainty=Algo::computeCertainty(n);


  // prepare output files/dirs
  ElementHolder holder(log_);
  // save root path
  const path root=createTempDirectory("/tmp");
  holder.push_back(root);
  // create all files
  Algo::forEachUniqueLeaf( n, FunctorIDMEF(log_, holder, root) );

  // run command in shell
  stringstream ss;
  ss << cfg_.getPath() << " " << root << " " << count << " " << priority << " " << certainty;
  LOGMSG_DEBUG_S(log_)<<"running command: "<< ss.str();
  const int ret=system( ss.str().c_str() );
  if(ret==-1)
  {
    LOGMSG_ERROR_S(log_)<<"unable to run command: " << ss.str();
    throw ExceptionCommandError(SYSTEM_SAVE_LOCATION, ss.str() );
  }
  // check for error code returned from command
  if(ret!=0)
  {
    LOGMSG_WARN_S(log_)<<"command returned error code " << ret << "; command was: " << ss.str();
    throw ExceptionCommandError(SYSTEM_SAVE_LOCATION, ss.str(), ret);
  }
}

} // namespace ExtApp
} // namespace Trigger
