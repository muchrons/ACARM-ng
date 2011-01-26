/*
 * IODynamicConfigCounter.cpp
 *
 */
#include <tut.h>

#include "Commons/Convert.hpp"
#include "TestHelpers/Persistency/IODynamicConfigCounter.hpp"

namespace TestHelpers
{
namespace Persistency
{

IODynamicConfigCounter::IODynamicConfigCounter(::Persistency::IO::Transaction &t, const std::string &keyValue):
  ::Persistency::IO::DynamicConfig("some owner", t),
  keyValue_(keyValue),
  isNull_(true),
  id_(1234567890)
  {
  }

void IODynamicConfigCounter::writeImpl(::Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
{
  tut::ensure_equals("invalid key to be written", key.get(), keyValue_);
  typedef ::Persistency::MetaAlert::ID::Numeric NumericID;
  const NumericID newID=Commons::Convert::to<NumericID>( value.get() );
  if(isNull_)
    tut::ensure_equals("invalid initial value to be written", newID, 1u);
  else
    tut::ensure_equals("invalid new ID to be written", newID, id_+1);
  id_    =newID;
  isNull_=false;
}

IODynamicConfigCounter::ValueNULL IODynamicConfigCounter::readImpl(::Persistency::IO::Transaction &/*t*/, const Key &key)
{
  tut::ensure_equals("invalid key to be read", key.get(), keyValue_);
  if(isNull_)
    return ValueNULL();
  return ValueNULL( Commons::Convert::to<std::string>(id_) );
}

IODynamicConfigCounter::Value IODynamicConfigCounter::readConstImpl(::Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
{
  tut::fail("readConst() should NOT be called at all");
  return Value("???");
}

void IODynamicConfigCounter::removeImpl(::Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
{
  tut::fail("remove() should NOT be called at all");
}

void IODynamicConfigCounter::iterateImpl(::Persistency::IO::Transaction &/*t*/, IterationCallback &/*cb*/)
{
  tut::fail("iterate() should NOT be called at all");
}

} // namespace Persistency
} // namespace TestHelpers
