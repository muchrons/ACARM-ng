/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <cstring>
#include <cmath>

#include "Algo/computeCertainty.hpp"
#include "Algo/computeSeverity.hpp"
#include "Commons/Convert.hpp"
#include "Commons/Filesystem/openFile.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Trigger/ExtApp/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace Trigger::ExtApp;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    th_("1.2", "0"),
    cfg_("testdata/logging_script", th_)
  {
  }

  ~TestClass(void)
  {
    try
    {
      // remove created elements if present
      if( !cleanUpDir_.empty() )
        remove_all(cleanUpDir_);
      remove("ext.log");            // removelog file, if exists
    }
    catch(...)
    {
      // nothing can bo done here
    }
  }

  template<typename T>
  void ensureThrows(const Config &c) const
  {
    Strategy               s("throwrunner", c);
    Strategy::ChangedNodes nc;
    try
    {
      s.process( makeNewLeaf(), nc );
      tut::fail("process() didn't failed on error condition");
    }
    catch(const T &)
    {
      // this is expected
    }
  }

  void ensureValid(GraphNodePtrNN node, const vector<path> &expectedFiles)
  {
    tut::ensure("no files expected?! error in test!", expectedFiles.size()>0u );
    // run application saving parameters
    Strategy               s("apprunner", cfg_);
    Strategy::ChangedNodes nc;
    remove("ext.log");          // ensure file is not present from previous runs
    s.process(node, nc);
    // prepare results for processing
    const double           priority =Algo::computeSeverity(node);
    const double           certainty=Algo::computeCertainty(node);
    const vector<string>   data     =parseLog();
    tut::ensure_equals("invalid number of paramters", data.size(), 5u);
    const path root=data[1];
    cleanUpDir_=root;
    // test paramters' values
    tut::ensure_equals("invalid application", data[0], system_complete("testdata/logging_script") );
    tut::ensure_equals("invalid alerts count",  Commons::Convert::to<size_t>(data[2]), expectedFiles.size() );
    tut::ensure("invalid priority", fabs(Commons::Convert::to<double>(data[3])-priority)<0.01 );
    tut::ensure("invalid certainty", fabs(Commons::Convert::to<double>(data[4])-certainty)<0.01 );
  }

  vector<string> parseLog(void) const
  {
    // open output file
    Commons::SharedPtrNotNULL<fstream> f=Commons::Filesystem::openFile("ext.log");
    string line;
    getline(*f, line);
    // parse elements
    typedef boost::char_separator<char> Separator;
    typedef boost::tokenizer<Separator> Tokenizer;
    const Separator sep(" ");
    const Tokenizer tokens(line, sep);
    // got through all tokens (i.e. node names)
    vector<string>  out;
    for(Tokenizer::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
      out.push_back(*it);
    // return tokens
    return out;
  }

  const Trigger::Simple::ThresholdConfig th_;
  const Config                           cfg_;
  path                                   cleanUpDir_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/ExtApp/Strategy");
} // unnamed namespace


namespace tut
{

// check if type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s("myscriptrunner", cfg_);
  ensure_equals("invalid type", s.getTriggerType(), "extapp");
}

// test exception on application call fail
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrows<Trigger::ExtApp::Exception>( Config( "/bin/false", cfg_.getThresholdConfig() ) );
}

// test processing single alert (leaf)
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN node=makeNewLeaf( makeNewAlert(), 1234u );
  vector<path>   files;
  files.push_back("idmef_1234.xml");
  ensureValid(node, files);
}

// test processing meta-alert (node)
template<>
template<>
void testObj::test<4>(void)
{
  GraphNodePtrNN node=makeNewNode( makeNewLeaf( makeNewAlert(), 10u ), makeNewLeaf( makeNewAlert(), 20u ), 666u );
  vector<path>   files;
  files.push_back("idmef_10.xml");
  files.push_back("idmef_20.xml");
  ensureValid(node, files);
}

// test processing meta-alert that does not match criteria
template<>
template<>
void testObj::test<5>(void)
{
  GraphNodePtrNN         node=makeNewLeaf();
  Strategy               s("apprunner", Config(cfg_.getPath(), Trigger::Simple::ThresholdConfig("99", "9") ) );
  Strategy::ChangedNodes nc;
  remove("ext.log");          // ensure file is not present from previous runs
  s.process(node, nc);
  ensure("trigger called", exists("ext.log")==false );
}

} // namespace tut
