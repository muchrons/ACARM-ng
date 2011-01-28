/*
 * IODynamicConfigCounter.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_IODYNAMICCONFIGCOUNTER_HPP
#define INCLUDE_TESTHELPERS_PERSISTENCY_IODYNAMICCONFIGCOUNTER_HPP

/* public header */

#include <string>

#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace TestHelpers
{
namespace Persistency
{

struct IODynamicConfigCounter: public ::Persistency::IO::DynamicConfig
{
  IODynamicConfigCounter(::Persistency::IO::Transaction &t, const std::string &keyValue);

  virtual void writeImpl(::Persistency::IO::Transaction &t, const Key &key, const Value &value);
  virtual ValueNULL readImpl(::Persistency::IO::Transaction &t, const Key &key);
  virtual Value readConstImpl(::Persistency::IO::Transaction &t, const Key &key);
  virtual void removeImpl(::Persistency::IO::Transaction &t, const Key &key);
  virtual void iterateImpl(::Persistency::IO::Transaction &t, IterationCallback &cb);

  const std::string                     keyValue_;
  bool                                  isNull_;
  ::Persistency::MetaAlert::ID::Numeric id_;
}; // struct IODynamicConfigCounter

} // namespace Persistency
} // namespace TestHelpers

#endif
