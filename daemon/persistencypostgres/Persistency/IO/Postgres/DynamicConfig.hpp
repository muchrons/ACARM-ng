/*
 * DynamicConfig.t.cpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DYNAMICCONFIG_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DYNAMICCONFIG_HPP_FILE

#include "Logger/Node.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief postgres implementaiton of DynamicConfig
 */
struct DynamicConfig: public Persistency::IO::DynamicConfig
{
public:
  /** \brief create instance for a given owner.
   *  \param owner owner's name.
   *  \param t     transaction to use.
   */
  DynamicConfig(const Owner &owner, Persistency::IO::Transaction &t);

private:
  virtual void writeImpl(Persistency::IO::Transaction &t, const Key &key, const Value &value);
  virtual ValueNULL readImpl(Persistency::IO::Transaction &t, const Key &key);
  virtual Value readConstImpl(Persistency::IO::Transaction &t, const Key &key);

  Logger::Node log_;
}; // class DynamicConfig

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
