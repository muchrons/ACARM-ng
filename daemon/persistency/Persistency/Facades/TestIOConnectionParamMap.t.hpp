/*
 * TestIOConnectionParamMap.t.hpp
 *
 */
#ifndef INCLUDE_TESTIOCONNECTIONPARAMMAP_T_HPP_FILE
#define INCLUDE_TESTIOCONNECTIONPARAMMAP_T_HPP_FILE

#include "Persistency/IO/IOStubs.t.hpp"

namespace
{

// connection that returns different implementaitons of the same counter
struct TestIOConnectionParamMap: public TestIOConnection
{
  // initialize internal counters
  virtual Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const Persistency::IO::DynamicConfig::Owner &/*owner*/,
                                                                  Persistency::IO::Transaction                &t)
  {
    return Persistency::IO::DynamicConfigAutoPtr( new IODynamicConfigMap(t, mem_) );
  }

  IODynamicConfigMap::Memory mem_;
}; // struct TestIOConnectionParamMap

} // unnamed namespace

#endif
