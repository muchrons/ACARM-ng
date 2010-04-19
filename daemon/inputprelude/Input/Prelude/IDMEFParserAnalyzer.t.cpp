/*
 * IDMEFParserAnalyzer.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

// TODO: "prelude-client.h"
#include <prelude-client.h>
#include "Input/Exception.hpp"
#include "Input/Prelude/IDMEFParserAnalyzer.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;

namespace
{

struct TestClass
{
  TestClass():
    name_("The Analyzer of Luke Skywaker"),
    ostype_("Wojtek linux"),
    osversion_("2.6.129 gr-sec"),
    address_("156.117.92.22")
  {
    if (idmef_analyzer_new(&analyzer_)<0)
      tut::fail("Unable to create analyzer obcject.");

    prelude_string_t *ps_name;
    prelude_string_t *ps_ostype;
    prelude_string_t *ps_osversion;
    prelude_string_t *ps_address;

    prelude_string_new_dup(&ps_name,name_.c_str());
    prelude_string_new_dup(&ps_ostype,ostype_.c_str());
    prelude_string_new_dup(&ps_osversion,osversion_.c_str());
    prelude_string_new_dup(&ps_address,address_.c_str());

    idmef_analyzer_set_name(analyzer_,ps_name);
    idmef_analyzer_set_ostype(analyzer_,ps_ostype);
    idmef_analyzer_set_osversion(analyzer_,ps_osversion);

    idmef_node_t *node;
    idmef_analyzer_new_node(analyzer_, &node);
    idmef_address_t * addr;

    idmef_node_new_address(node,&addr,IDMEF_LIST_APPEND);
    idmef_address_set_address(addr,ps_address);
    idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
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
  std::string name_;
  std::string ostype_;
  std::string osversion_;
  std::string address_;
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
  ensure(an.getName()==Persistency::Analyzer::Name(name_));
}

// OS extraction test
template<>
template<>
void testObj::test<2>(void)
{
  IDMEFParserAnalyzer an(getAnalyzer());
  std::cerr << *an.getOS() << ' ' << std::endl;
  //ensure_equals("asd",*an.getOS(),Persistency::Analyzer::OS(ostype_));
}

// TODO: test passing NULL pointer

// TODO: test parsing analyzer without name

// TODO: test parsing analyzer without version

// TODO: test parsing analyzer without ostype

// TODO: test parsing analyzer without osversion

// TODO: test parsing analyzer without osname

// TODO: test parsing analyzer without address

// TODO: test parsing analyzer with IPv4 address

// TODO: test parsing analyzer with IPv6 address

} // namespace tut
