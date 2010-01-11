/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTIONBUILDER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTIONBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/IDCache.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief builder for reader of prelude format.
 */
class ConnectionBuilder: public BackendFactory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a object's facotry for Persistency::Postgres.
   */
  ConnectionBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const std::string  name_;
  const Logger::Node log_;
  IDCachePtrNN       idCache_;
}; // class ConnectionBuilder

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
