/*
 * IDMEFParserSource.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

#include <prelude-client.h>
#include "Input/Exception.hpp"
#include "Input/Prelude/IDMEFParserSource.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;

namespace
{

struct TestClass
{
  TestClass()
  {
    if (idmef_source_new(&source_)<0)
      tut::fail("Unable to create source obcject.");


    prelude_string_t *addr_str;
    prelude_string_new_dup(&addr_str,"192.168.55.54");

    idmef_address_t *addr;
    idmef_address_new(&addr);
    idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
    idmef_address_set_address(addr,addr_str);

    idmef_node_t *node;
    idmef_source_new_node(source_, &node);

    idmef_node_set_address(node,addr,IDMEF_LIST_APPEND);

    prelude_string_t *proc_str;
    prelude_string_new_dup(&proc_str,"some process");
    /*
    idmef_process_t *proc;
    idmef_process_new(&proc);
    idmef_process_set_name(proc,proc_str);
    idmef_source_set_process(source_,proc);
    */
  }

  ~TestClass()
  {
    idmef_source_destroy(source_);
  }

  idmef_source_t * getSource()
  {
    return source_;
  }

protected:
  idmef_source_t *source_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParserSource");
} // unnamed namespace


namespace tut
{
template<>
template<>
void testObj::test<1>(void)
{
  IDMEFParserSource ips(getSource());
  ensure_equals("Address IPv4",ips.getAddress(),Persistency::Analyzer::IP(boost::asio::ip::address_v4::from_string("192.168.55.54")));
}

template<>
template<>
void testObj::test<2>(void)
{
  char addrv6[]="2001:0db8:0000:0000:0000:0000:1428:57ab";
  idmef_source_t *source6;
  idmef_source_new(&source6);
  prelude_string_t *addr_str;
  prelude_string_new_dup(&addr_str,addrv6);

  idmef_address_t *addr;
  idmef_address_new(&addr);
  idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV6_ADDR);
  idmef_address_set_address(addr,addr_str);

  idmef_node_t *node;
  idmef_source_new_node(source6, &node);

  idmef_node_set_address(node,addr,IDMEF_LIST_APPEND);

  IDMEFParserSource ips(source6);
  ensure_equals("Address IPv6",ips.getAddress(),Persistency::Analyzer::IP(boost::asio::ip::address_v6::from_string(addrv6)));
}


template<>
template<>
void testObj::test<3>(void)
{

}


} // namespace tut
