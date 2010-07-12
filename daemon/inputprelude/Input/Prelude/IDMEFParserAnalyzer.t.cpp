/*
 * IDMEFParserAnalyzer.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <memory>
#include <cstring>
#include <prelude-client.h>

#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParserAnalyzer.hpp"
#include "TestHelpers/Input/TestBase.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;
using namespace Persistency;

// TODO: rework this code using Input::Prelude::TestData

namespace
{

struct TestClass: TestHelpers::Input::TestBase
{
  TestClass():
    id_("Death/Star/id/42"),
    name_("The Analyzer of Luke Skywaker"),
    ostype_("Wojtek linux"),
    osversion_("2.6.129 gr-sec"),
    address_("156.117.92.22")
  {
    if (idmef_analyzer_new(&analyzer_)<0)
      tut::fail("Unable to create analyzer object.");

    prelude_string_t *ps_id       =NULL;
    prelude_string_t *ps_name     =NULL;
    prelude_string_t *ps_ostype   =NULL;
    prelude_string_t *ps_osversion=NULL;
    prelude_string_t *ps_address  =NULL;

    prelude_string_new_dup(&ps_id, id_.c_str() );
    prelude_string_new_dup(&ps_name, name_.c_str() );
    prelude_string_new_dup(&ps_ostype, ostype_.c_str() );
    prelude_string_new_dup(&ps_osversion, osversion_.c_str() );
    prelude_string_new_dup(&ps_address, address_.c_str() );

    idmef_analyzer_set_analyzerid(analyzer_, ps_id);
    idmef_analyzer_set_name(analyzer_, ps_name);
    idmef_analyzer_set_ostype(analyzer_, ps_ostype);
    idmef_analyzer_set_osversion(analyzer_, ps_osversion);

    idmef_node_t *node=NULL;
    idmef_analyzer_new_node(analyzer_, &node);

    idmef_address_t *addr=NULL;
    idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
    idmef_address_set_address(addr, ps_address);
    idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
  }

  ~TestClass()
  {
    idmef_analyzer_destroy(analyzer_);
  }

  idmef_analyzer_t * getAnalyzer()
  {
    return analyzer_;
  }

protected:
  idmef_analyzer_t *analyzer_;
  std::string       id_;
  std::string       name_;
  std::string       ostype_;
  std::string       osversion_;
  std::string       address_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParserAnalyzer");
} // unnamed namespace


namespace tut
{

// Name extraction from Analyzer
template<>
template<>
void testObj::test<1>(void)
{
  IDMEFParserAnalyzer an(getAnalyzer());
  ensure_equals("Analyzer name:",an.getName().get(),name_);
}

// OS extraction test
template<>
template<>
void testObj::test<2>(void)
{
  IDMEFParserAnalyzer an(getAnalyzer());
  ensure_equals("OS version:", std::string(an.getOS().get()),ostype_+osversion_);
}

// address extracion (IPv4)
template<>
template<>
void testObj::test<3>(void)
{
  IDMEFParserAnalyzer an(getAnalyzer());
  ensure_equals("IP address",*an.getIP(),boost::asio::ip::address_v4::from_string(address_));
}

// address extracion (IPv6)
template<>
template<>
void testObj::test<4>(void)
{
  idmef_analyzer_t *analyzer;
  if (idmef_analyzer_new(&analyzer)<0)
    tut::fail("Unable to create analyzer object.");

  prelude_string_t *ps_id;
  prelude_string_t *ps_name;
  prelude_string_t *ps_ostype;
  prelude_string_t *ps_osversion;
  prelude_string_t *ps_address;

  prelude_string_new_dup(&ps_id, id_.c_str() );
  prelude_string_new_dup(&ps_name, name_.c_str() );
  prelude_string_new_dup(&ps_ostype, ostype_.c_str() );
  prelude_string_new_dup(&ps_osversion, osversion_.c_str() );
  prelude_string_new_dup(&ps_address, "::1");

  idmef_analyzer_set_analyzerid(analyzer, ps_id);
  idmef_analyzer_set_name(analyzer, ps_name);
  idmef_analyzer_set_ostype(analyzer, ps_ostype);
  idmef_analyzer_set_osversion(analyzer, ps_osversion);

  idmef_node_t *node;
  idmef_analyzer_new_node(analyzer, &node);
  idmef_address_t * addr;

  idmef_node_new_address(node,&addr,IDMEF_LIST_APPEND);
  idmef_address_set_address(addr,ps_address);
  idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV6_ADDR);

  IDMEFParserAnalyzer an(analyzer);
  ensure_equals("IP address:",*an.getIP(),boost::asio::ip::address_v6::from_string("::1"));
  idmef_analyzer_destroy(analyzer);
}


// NULL pointer test
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    IDMEFParserAnalyzer an(NULL);
    fail("Exception was not thrown.");
  }
  catch(const ExceptionParse &e)
  {
    //expected
  }
}

// analyzer without a name
template<>
template<>
void testObj::test<6>(void)
{
  idmef_analyzer_t *analyzer;
  if (idmef_analyzer_new(&analyzer)<0)
    tut::fail("Unable to create analyzer object.");

  prelude_string_t *ps_id;
  prelude_string_t *ps_ostype;
  prelude_string_t *ps_osversion;
  prelude_string_t *ps_address;

  prelude_string_new_dup(&ps_id, id_.c_str() );
  prelude_string_new_dup(&ps_ostype, ostype_.c_str() );
  prelude_string_new_dup(&ps_osversion, osversion_.c_str() );
  prelude_string_new_dup(&ps_address, address_.c_str() );   // TODO: memleak

  idmef_analyzer_set_analyzerid(analyzer, ps_id);
  idmef_analyzer_set_ostype(analyzer, ps_ostype);
  idmef_analyzer_set_osversion(analyzer, ps_osversion);

  idmef_node_t *node;
  idmef_analyzer_new_node(analyzer, &node);
  idmef_address_t * addr;

  idmef_node_new_address(node,&addr,IDMEF_LIST_APPEND);
  idmef_address_set_address(addr,ps_address);
  idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);

  IDMEFParserAnalyzer an(analyzer);
  ensure_equals("Name",std::string(an.getName().get()),"Unknown");
  idmef_analyzer_destroy(analyzer);
}

// analyser without an osversion or ostype
template<>
template<>
void testObj::test<7>(void)
{
  idmef_analyzer_t *analyzer;
  if (idmef_analyzer_new(&analyzer)<0)
    tut::fail("Unable to create analyzer object.");

  prelude_string_t *ps_id;
  prelude_string_t *ps_name;
  prelude_string_t *ps_address;

  prelude_string_new_dup(&ps_id, id_.c_str() );
  prelude_string_new_dup(&ps_name, name_.c_str() );
  prelude_string_new_dup(&ps_address, address_.c_str() );   // TODO: mem-leak

  idmef_analyzer_set_analyzerid(analyzer, ps_id);
  idmef_analyzer_set_name(analyzer, ps_name);

  idmef_node_t *node;
  idmef_analyzer_new_node(analyzer, &node);
  idmef_address_t * addr;

  idmef_node_new_address(node,&addr,IDMEF_LIST_APPEND);
  idmef_address_set_address(addr,ps_address);
  idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);

  IDMEFParserAnalyzer an(analyzer);
  ensure(an.getOS()==NULL);
  idmef_analyzer_destroy(analyzer);
}

// analyzer without an address
template<>
template<>
void testObj::test<8>(void)
{
  idmef_analyzer_t *analyzer;
  if (idmef_analyzer_new(&analyzer)<0)
    tut::fail("Unable to create analyzer object.");

  prelude_string_t *ps_id;
  prelude_string_t *ps_name;
  prelude_string_t *ps_ostype;
  prelude_string_t *ps_osversion;

  prelude_string_new_dup(&ps_id, id_.c_str() );
  prelude_string_new_dup(&ps_name, name_.c_str() );
  prelude_string_new_dup(&ps_ostype, ostype_.c_str() );
  prelude_string_new_dup(&ps_osversion, osversion_.c_str() );

  idmef_analyzer_set_analyzerid(analyzer, ps_id);
  idmef_analyzer_set_name(analyzer, ps_name);
  idmef_analyzer_set_ostype(analyzer, ps_ostype);
  idmef_analyzer_set_osversion(analyzer, ps_osversion);

  IDMEFParserAnalyzer an(analyzer);
  idmef_analyzer_destroy(analyzer);
}

// wrong address specification
template<>
template<>
void testObj::test<9>(void)
{
  idmef_analyzer_t *analyzer;
  if (idmef_analyzer_new(&analyzer)<0)
    tut::fail("Unable to create analyzer object.");

  prelude_string_t *ps_name;
  prelude_string_t *ps_ostype;
  prelude_string_t *ps_osversion;
  prelude_string_t *ps_address;

  prelude_string_new_dup(&ps_name,name_.c_str());
  prelude_string_new_dup(&ps_ostype,ostype_.c_str());
  prelude_string_new_dup(&ps_osversion,osversion_.c_str());
  prelude_string_new_dup(&ps_address,address_.c_str());

  idmef_analyzer_set_name(analyzer,ps_name);
  idmef_analyzer_set_ostype(analyzer,ps_ostype);
  idmef_analyzer_set_osversion(analyzer,ps_osversion);

  idmef_node_t *node;
  idmef_analyzer_new_node(analyzer, &node);
  idmef_address_t * addr;

  idmef_node_new_address(node,&addr,IDMEF_LIST_APPEND);
  idmef_address_set_address(addr,ps_address);
  idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV6_ADDR);

  try
  {
    IDMEFParserAnalyzer an(analyzer);
    fail("Exception was not thrown.");
  }
  // TODO: fix this catch - specifc exception must be given here, otherwise
  //       tut::fail()'s exceptions are catched as well and test does not
  //       test anything.
  catch(...)
  {
    //expected
  }
  idmef_analyzer_destroy(analyzer);
}

// extract ID from analyzer
template<>
template<>
void testObj::test<10>(void)
{
  IDMEFParserAnalyzer an( getAnalyzer() );
  ensure_equals("invalid ID", an.getPreludeID(), id_);
}

// test for fallback when ID's not set
template<>
template<>
void testObj::test<11>(void)
{
  idmef_analyzer_t *analyzer=NULL;
  if (idmef_analyzer_new(&analyzer)<0)
    tut::fail("Unable to create analyzer object.");

  prelude_string_t *ps_name     =NULL;
  prelude_string_t *ps_ostype   =NULL;
  prelude_string_t *ps_osversion=NULL;
  prelude_string_t *ps_address  =NULL;

  prelude_string_new_dup(&ps_name, name_.c_str() );
  prelude_string_new_dup(&ps_ostype, ostype_.c_str() );
  prelude_string_new_dup(&ps_osversion, osversion_.c_str() );
  prelude_string_new_dup(&ps_address, address_.c_str() );

  idmef_analyzer_set_name(analyzer, ps_name);
  idmef_analyzer_set_ostype(analyzer, ps_ostype);
  idmef_analyzer_set_osversion(analyzer, ps_osversion);

  idmef_node_t *node=NULL;
  idmef_analyzer_new_node(analyzer, &node);

  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, ps_address);
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);

  IDMEFParserAnalyzer an(analyzer);
  ensure_equals("invalid value returned",
                an.getPreludeID(),
                "UnknownID/The Analyzer of Luke Skywaker/NULLSTR/Wojtek linux2.6.129 gr-sec/156.117.92.22");

  idmef_analyzer_destroy(analyzer);
}

} // namespace tut
