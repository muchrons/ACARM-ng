/*
 * DynamicConfig.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_DYNAMICCONFIG_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_DYNAMICCONFIG_HPP_FILE

#include <map>
#include <string>

#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief stub of DynamicConfig.
 */
struct DynamicConfig: public Persistency::IO::DynamicConfig
{
public:
  /** \brief create instance.
   *  \param owner  owner's name.
   *  \param t      transaction to use.
   *  \param handle persistency handle.
   */
  DynamicConfig(const Owner &owner, Persistency::IO::Transaction &t, int handle);

  int callsWrite_;      ///< calls to write()
  int callsRead_;       ///< calls to read()
  int callsReadConst_;  ///< calls to readConst()

private:
  virtual void writeImpl(Persistency::IO::Transaction &t, const Key &key, const Value &value);
  virtual ValueNULL readImpl(Persistency::IO::Transaction &t, const Key &key);
  virtual Value readConstImpl(Persistency::IO::Transaction &t, const Key &key);
  virtual void removeImpl(Persistency::IO::Transaction &t, const Key &key);

  typedef std::map<std::string, std::string> DataMap;
  typedef std::map<std::string, DataMap>     OwnerMap;

  OwnerMap owner_;
}; // class DynamicConfig

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
