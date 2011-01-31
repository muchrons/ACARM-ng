/*
 * DynamicConfig.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DYNAMICCONFIG_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DYNAMICCONFIG_HPP_FILE

#include "Logger/Node.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Persistency/IO/Postgres/DBHandle.hpp"

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
   *  \param owner    owner's name.
   *  \param t        transaction to use.
   *  \param dbHandle data base handle.
   */
  DynamicConfig(const Owner                  &owner,
                Persistency::IO::Transaction &t,
                DBHandlePtrNN                 dbHandle);

private:
  virtual void writeImpl(Persistency::IO::Transaction &t, const Key &key, const Value &value);
  virtual ValueNULL readImpl(Persistency::IO::Transaction &t, const Key &key);
  virtual Value readConstImpl(Persistency::IO::Transaction &t, const Key &key);
  virtual void removeImpl(Persistency::IO::Transaction &t, const Key &key);
  virtual void iterateImpl(Persistency::IO::Transaction &t, IterationCallback &cb);

  Logger::Node  log_;
  DBHandlePtrNN dbHandle_;
}; // class DynamicConfig

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
