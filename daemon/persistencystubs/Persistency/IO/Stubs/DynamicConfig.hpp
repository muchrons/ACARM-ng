/*
 * DynamicConfig.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_DYNAMICCONFIG_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_DYNAMICCONFIG_HPP_FILE

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
   *  \param owner owner's name.
   *  \param t     transaction to use.
   */
  DynamicConfig(const Owner &owner, Persistency::IO::Transaction &t);

  int callsWrite_;      ///< calls to write()
  int callsRead_;       ///< calls to read()
  int callsReadConst_;  ///< calls to readConst()

private:
  virtual void writeImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/, const Value &/*value*/);
  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/);
  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/);
}; // class DynamicConfig

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
