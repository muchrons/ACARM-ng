/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <utility>

#include "Trigger/SnortSam/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;
using namespace Trigger::SnortSam;
using namespace TestHelpers::Persistency;

namespace
{

struct ProtocolMock: public Protocol
{
public:
  typedef std::pair<Config::IP, Config::IP> BlockReq;   // <from,to>
  typedef std::vector<BlockReq>             Requests;
  Requests r_;

private:
  virtual const std::string &getProtocolVersionImpl(void) const
  {
    static const std::string v("66.6");
    return v;
  }
  virtual bool isConnectedImpl(void) const
  {
    return true;
  }
  virtual void initImpl(void)
  {
  }
  virtual void deinitImpl(void)
  {
  }
  virtual void blockImpl(const Config::IP &from, const Config::IP &to)
  {
    r_.push_back( BlockReq(from,to) );
  }
}; // struct ProtocolMock


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    pm_(new ProtocolMock),
    apm_(pm_),
    cfg_("127.0.0.1", 4242, "secret", Who::SRC, How::IN, 666, Trigger::Simple::ThresholdConfig("0", "0") ),
    s_(InstanceName("trigsamtest"), cfg_, apm_)
  {
    assert(pm_!=NULL);
  }

  ProtocolMock              *pm_;
  Strategy::ProtocolAutoPtr  apm_;
  Config                     cfg_;
  Strategy                   s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Strategy");
} // unnamed namespace


namespace tut
{

// check if trigger type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid trigger type", s_.getTriggerType().str(), "snortsam");
}

// test NULL protocol
template<>
template<>
void testObj::test<2>(void)
{
  ensure("pre-condition failed", apm_.get()==NULL);
  try
  {
    Strategy s(InstanceName("iwillthrow"), cfg_, apm_); // should throw
    fail("c-tor didn't failed on NULL pointer");
  }
  catch(const Trigger::SnortSam::Exception &)
  {
    // this is expected
  }
}

// test trigger from simple alert
template<>
template<>
void testObj::test<3>(void)
{
  // send report
  Strategy::ChangedNodes nc;
  s_.process( makeNewLeaf("1.2.3.4", "5.6.7.8"), nc );
  // check results
  ensure_equals("invalid number of blocks sent", pm_->r_.size(), 1u);
  ensure_equals("invalid source IP", pm_->r_.at(0).first.to_string(),  "1.2.3.4");
  ensure_equals("invalid target IP", pm_->r_.at(0).second.to_string(), "5.6.7.8");
}

// test triggering more complicated meta-alert
template<>
template<>
void testObj::test<4>(void)
{
  // send report
  Strategy::ChangedNodes nc;
  s_.process( makeNewNode( makeNewLeaf("1.2.3.4", "5.6.7.8"), makeNewLeaf("9.8.7.6", "5.4.3.2") ), nc );
  // check results
  ensure_equals("invalid number of blocks sent", pm_->r_.size(), 4u);
  // 1->1
  ensure_equals("invalid source IP/0", pm_->r_.at(0).first.to_string(),  "1.2.3.4");
  ensure_equals("invalid target IP/0", pm_->r_.at(0).second.to_string(), "5.4.3.2");
  // 1->2
  ensure_equals("invalid source IP/1", pm_->r_.at(1).first.to_string(),  "1.2.3.4");
  ensure_equals("invalid target IP/1", pm_->r_.at(1).second.to_string(), "5.6.7.8");
  // 2->1
  ensure_equals("invalid source IP/2", pm_->r_.at(2).first.to_string(),  "9.8.7.6");
  ensure_equals("invalid target IP/2", pm_->r_.at(2).second.to_string(), "5.4.3.2");
  // 2->2
  ensure_equals("invalid source IP/3", pm_->r_.at(3).first.to_string(),  "9.8.7.6");
  ensure_equals("invalid target IP/3", pm_->r_.at(3).second.to_string(), "5.6.7.8");
}

// test if IPs do not repeat, when not needed
template<>
template<>
void testObj::test<5>(void)
{
  // send report
  Strategy::ChangedNodes nc;
  s_.process( makeNewNode( makeNewLeaf("1.2.3.4", "5.6.7.8"), makeNewLeaf("1.2.3.4", "5.4.3.2") ), nc );
  // check results
  ensure_equals("invalid number of blocks sent", pm_->r_.size(), 2u);
  // 1->1
  ensure_equals("invalid source IP/0", pm_->r_.at(0).first.to_string(),  "1.2.3.4");
  ensure_equals("invalid target IP/0", pm_->r_.at(0).second.to_string(), "5.4.3.2");
  // 1->2
  ensure_equals("invalid source IP/1", pm_->r_.at(1).first.to_string(),  "1.2.3.4");
  ensure_equals("invalid target IP/1", pm_->r_.at(1).second.to_string(), "5.6.7.8");
}

} // namespace tut
