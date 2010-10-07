/*
 * IODynamicConfigMemory.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_IODYNAMICCONFIGMEMORY_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_IODYNAMICCONFIGMEMORY_HPP_FILE

/* public header */

#include <map>
#include <string>

#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

namespace TestHelpers
{
namespace Persistency
{

struct IODynamicConfigMemory: public ::Persistency::IO::DynamicConfig
{
public:
  typedef std::map<std::string, std::string> Memory;

  IODynamicConfigMemory(const ::Persistency::IO::DynamicConfig::Owner &owner,
                        ::Persistency::IO::Transaction                &t,
                        Memory                                        &mem);

  virtual void writeImpl(::Persistency::IO::Transaction &t, const Key &key, const Value &value);
  virtual ValueNULL readImpl(::Persistency::IO::Transaction &t, const Key &key);
  virtual Value readConstImpl(::Persistency::IO::Transaction &t, const Key &key);

  Memory &mem_;
}; // class IODynamicConfigMemory

} // namespace Persistency
} // namespace TestHelpers

#endif
