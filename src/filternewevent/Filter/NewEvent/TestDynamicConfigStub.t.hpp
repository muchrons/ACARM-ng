/*
 * TestDynamicConfigStub.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_TESTDYNAMICCONFIGSTUB_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_TESTDYNAMICCONFIGSTUB_HPP_FILE

/* public header */

#include <map>
#include <cassert>

#include "Persistency/IO/DynamicConfig.hpp"

namespace Filter
{
namespace NewEvent
{

struct TestDynamicConfigStub: public Persistency::IO::DynamicConfig
{
public:
  // helper strucutre to allow easy access
  struct Data
  {
    typedef std::map<std::string, std::string> DataMap;
    typedef std::map<std::string, DataMap>     OwnerMap;

    OwnerMap owner_;
  }; // struct Data
  typedef boost::shared_ptr<Data> DataPtr;

  TestDynamicConfigStub(const Owner &owner, Persistency::IO::Transaction &t):
    Persistency::IO::DynamicConfig(owner, t),
    data_(new Data)
  {
  }

  TestDynamicConfigStub(const Owner &owner, Persistency::IO::Transaction &t, DataPtr data):
    Persistency::IO::DynamicConfig(owner, t),
    data_(data)
  {
  }

  DataPtr getDataPtr(void)
  {
    assert( data_.get()!=NULL );
    return data_;
  }

private:
  virtual void writeImpl(Persistency::IO::Transaction &t, const Key &key, const Value &value)
  {
    getDataMap()[ key.get() ]=value.get();
    assert( readImpl(t, key).get()!=NULL && "writing failed" );
  }

  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    Data::DataMap &data=getDataMap();
    if( data.find( key.get() )==data.end() )
      return ValueNULL();
    return ValueNULL( data[ key.get() ] );
  }

  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    throw std::runtime_error("TestDynamicConfigStub::readConstImpl() not implemented");
  }

  virtual void removeImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    getDataMap().erase( key.get() );
  }

  virtual void iterateImpl(Persistency::IO::Transaction &/*t*/, IterationCallback &cb)
  {
    for(Data::DataMap::const_iterator it=getDataMap().begin(); it!=getDataMap().end(); ++it)
      if( cb.process(it->first, it->second)==false )
        return;
  }

  Data::DataMap &getDataMap(void)
  {
    return getDataPtr()->owner_[ getOwner().get() ];
  }

  DataPtr data_;
}; // class TestDynamicConfigStub

} // namespace Filter
} // namespace NewEvent

#endif
