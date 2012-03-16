/*
 * IDMEFParserTarget.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

#include <prelude-client.h>
#include "Input/Exception.hpp"
#include "Input/Prelude/IDMEFParserTarget.hpp"
#include "Input/Prelude/TestData.t.hpp"
#include "TestHelpers/Input/TestBase.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;
using namespace Persistency;

namespace
{

struct TestClass: public TestHelpers::Input::TestBase
{
  TestData td_;

  TestClass()
  {
    td_.makeAlert();
    td_.addClassificationToAlert("some classification");
    td_.addAnalyzerToAlert();
    target_=td_.addTargetToAlert();
  }

protected:
  idmef_target_t * target_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParserTarget");
} // unnamed namespace


namespace tut
{

// Check address (IPv4)
template<>
template<>
void testObj::test<1>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  const IDMEFParserTarget ips(target_);
  ensure_equals("Address IPv4",ips.getAddress(),Analyzer::IP(boost::asio::ip::address_v4::from_string("192.168.0.4")));
}

// Check address (IPv6)
template<>
template<>
void testObj::test<2>(void)
{
  td_.addAddressToTarget(target_,"2001:0db8:0000:0000:0000:0000:1428:57ab",true);
  const IDMEFParserTarget ips(target_);
  ensure_equals("Address IPv4",ips.getAddress(),Analyzer::IP(boost::asio::ip::address_v6::from_string("2001:0db8:0000:0000:0000:0000:1428:57ab")));
}

// Check process name
template<>
template<>
void testObj::test<3>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  td_.addProcessToTarget(target_,"foooood");
  const IDMEFParserTarget ips(target_);
  ensure("Target process is null",ips.getProcess()!=NULL);
  ensure_equals("Process Name",ips.getProcess()->getName().get(),std::string("foooood"));
}


// Check process user
template<>
template<>
void testObj::test<4>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  td_.addProcessToTarget(target_,"foooood");
  td_.addUserToTarget(target_,"Alf");

  const IDMEFParserTarget ips(target_);
  ensure("Target process is null",ips.getProcess()!=NULL);
  ensure_equals("Process User",ips.getProcess()->getUsername().get(),std::string("Alf"));
}

// Check service name
template<>
template<>
void testObj::test<5>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  td_.addServiceToTarget(target_,"nazwa",NULL,80);

  const IDMEFParserTarget ips(target_);
  ensure("Target service is null",ips.getService()!=NULL);
  ensure_equals("Service Name",ips.getService()->getName().get(),std::string("nazwa"));
}

// Check service protocol name
template<>
template<>
void testObj::test<6>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  td_.addServiceToTarget(target_,"nazwa","proto",80);

  const IDMEFParserTarget ips(target_);
  ensure("Target service is null",ips.getService()!=NULL);
  ensure_equals("Service Protocol",ips.getService()->getProtocol().get(),std::string("proto"));
}

// Check service port 0
template<>
template<>
void testObj::test<7>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  td_.addServiceToTarget(target_,"nazwa","proto",0);

  const IDMEFParserTarget ips(target_);
  ensure("Target service is not-null",ips.getService()==NULL);
}

// Check service port nonzero
template<>
template<>
void testObj::test<8>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  td_.addServiceToTarget(target_,"nazwa","proto",42);

  const IDMEFParserTarget ips(target_);
  ensure("Target service is null",ips.getService()!=NULL);
  ensure_equals("Service Protocol",ips.getService()->getPort().get(),42);
}

// Check service with no name
template<>
template<>
void testObj::test<9>(void)
{
  td_.addAddressToTarget(target_,"192.168.0.4",false);
  td_.addServiceToTarget(target_,NULL,"proto",80);

  const IDMEFParserTarget ips(target_);
  ensure("Source service is null",ips.getService()!=NULL);
  ensure_equals("Service Name",ips.getService()->getName().get(), std::string("unknown"));
}

} // namespace tut
