/*
 * TestDynamicConfigStub.hpp
 *
 */
#ifndef INCLUDE_INPUT_TESTDYNAMICCONFIGSTUB_T_HPP_FILE
#define INCLUDE_INPUT_TESTDYNAMICCONFIGSTUB_T_HPP_FILE

/* public header */

#include <map>

#include "Persistency/IO/DynamicConfig.hpp"

namespace Input
{

struct TestDynamicConfigStub: public Persistency::IO::DynamicConfig
{
public:
  TestDynamicConfigStub(const Owner &owner, Persistency::IO::Transaction &t):
    Persistency::IO::DynamicConfig(owner, t)
  {
  }

private:
  virtual void writeImpl(Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
  {
    owner_[ getOwner().get() ][ key.get() ]=value.get();
  }

  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    DataMap &data=owner_[ getOwner().get() ];
    if( data.find( key.get() )==data.end() )
      return ValueNULL();
    return ValueNULL( data[ key.get() ] );
  }

  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    throw std::runtime_error("TestDynamicConfigStub::readConstImpl() not implemented");
  }

  typedef std::map<std::string, std::string> DataMap;
  typedef std::map<std::string, DataMap>     OwnerMap;

  OwnerMap owner_;
}; // class TestDynamicConfigStub

} // namespace Input

#endif
