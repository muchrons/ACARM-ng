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
using namespace Persistency;

namespace
{

struct TestClass
{
  TestClass():
    process_name("some_process"),
    process_user("wojek"),
    service_name("apache"),
    service_protocol("TCP"),
    service_port(27600),
    addressv4("192.168.55.54")
  {
    if (idmef_source_new(&source_)<0)
      tut::fail("Unable to create source obcject.");

    prelude_string_t *addr_str;
    prelude_string_new_dup(&addr_str,addressv4.c_str());

    idmef_address_t *addr;
    idmef_address_new(&addr);
    idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
    idmef_address_set_address(addr,addr_str);

    idmef_node_t *node;
    idmef_source_new_node(source_, &node);

    idmef_node_set_address(node,addr,IDMEF_LIST_APPEND);

    prelude_string_t *proc_str;
    prelude_string_new_dup(&proc_str,process_name.c_str());

    idmef_process_t *proc;
    idmef_process_new(&proc);
    idmef_process_set_name(proc,proc_str);

    idmef_user_t *user;
    idmef_source_new_user(source_,&user);

    idmef_user_id_t* userid;
    idmef_user_new_user_id(user,&userid,IDMEF_LIST_APPEND);

    prelude_string_t *username;
    prelude_string_new_dup(&username,process_user.c_str());
    idmef_user_id_set_name(userid,username);

    idmef_source_set_process(source_,proc);

    idmef_service_t * service;
    idmef_source_new_service(source_,&service);

    prelude_string_t *servicename;
    prelude_string_new_dup(&servicename,service_name.c_str());
    idmef_service_set_name(service,servicename);

    prelude_string_t *protocol;
    prelude_string_new_dup(&protocol,service_protocol.c_str());
    idmef_service_set_protocol(service,protocol);

    idmef_service_set_port(service,27600);
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
  std::string process_name;
  std::string process_user;
  std::string service_name;
  std::string service_protocol;
  uint16_t service_port;
  std::string addressv4;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParserSource");
} // unnamed namespace


namespace tut
{

// Check address (IPv4)
template<>
template<>
void testObj::test<1>(void)
{
  IDMEFParserSource ips(getSource());
  ensure_equals("Address IPv4",ips.getAddress(),Analyzer::IP(boost::asio::ip::address_v4::from_string(addressv4)));
}

// Check address (IPv6)
template<>
template<>
void testObj::test<2>(void)
{
  char addrv6[]="2001:0db8:0000:0000:0000:0000:1428:57ab";
  idmef_source_t *source6;
  idmef_source_new(&source6);
  prelude_string_t *addr_str6;
  prelude_string_new_dup(&addr_str6,addrv6);

  idmef_address_t *addr6;
  idmef_address_new(&addr6);
  idmef_address_set_category(addr6,IDMEF_ADDRESS_CATEGORY_IPV6_ADDR);
  idmef_address_set_address(addr6,addr_str6);

  idmef_node_t *node6;
  idmef_source_new_node(source6, &node6);

  idmef_node_set_address(node6,addr6,IDMEF_LIST_APPEND);

  IDMEFParserSource ips(source6);
  ensure_equals("Address IPv6",ips.getAddress(),Analyzer::IP(boost::asio::ip::address_v6::from_string(addrv6)));
  idmef_source_destroy(source6);
}

//Check process name
template<>
template<>
void testObj::test<3>(void)
{
  IDMEFParserSource ips(getSource());
  ensure("Source process is null",ips.getProcess()!=NULL);
  ensure_equals("Process Name",ips.getProcess()->getName().get(),process_name);
}

//Check process user
template<>
template<>
void testObj::test<4>(void)
{
  IDMEFParserSource ips(getSource());
  ensure("Source process is null",ips.getProcess()!=NULL);
  ensure_equals("Process User",ips.getProcess()->getUsername().get(),process_user);
}

//Check service name
template<>
template<>
void testObj::test<5>(void)
{
  IDMEFParserSource ips(getSource());
  ensure("Source service is null",ips.getService()!=NULL);
  ensure_equals("Service Name",ips.getService()->getName().get(),service_name);
}

//Check service protocol
template<>
template<>
void testObj::test<6>(void)
{
  IDMEFParserSource ips(getSource());
  ensure("Source service is null",ips.getService()!=NULL);
  ensure_equals("Service Protocol",ips.getService()->getProtocol().get(),service_protocol);
}

//Check service port
template<>
template<>
void testObj::test<7>(void)
{
  IDMEFParserSource ips(getSource());
  ensure("Source service is null",ips.getService()!=NULL);
  ensure_equals("Service Port",ips.getService()->getPort(),service_port);
}


} // namespace tut
