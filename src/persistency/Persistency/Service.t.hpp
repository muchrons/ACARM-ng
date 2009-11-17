/*
 * Service.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_SERVICE_T_HPP_FILE
#define INCLUDE_PERSISTENCY_SERVICE_T_HPP_FILE

#include "Persistency/Service.hpp"
#include "Persistency/ReferenceURL.t.hpp"

namespace
{

struct ServiceTestImpl: public Service
{
  ServiceTestImpl(const char      *name,
                  const uint16_t   port,
                  const char      *protocol,
                  ReferenceURLPtr  url):
    Service(name, port, protocol, url)
  {
  }
}; // struct TestImpl

} // unnamed namespace

#endif
