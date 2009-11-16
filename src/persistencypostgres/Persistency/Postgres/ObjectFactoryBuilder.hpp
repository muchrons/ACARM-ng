/*
 * ObjectFactoryBuilder.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_OBJECTFACTORYBUILDER_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_OBJECTFACTORYBUILDER_HPP_FILE

/* public header */

#include <string>

#include "Logger/Logger.hpp"
#include "Persistency/BackendFactory.hpp"

namespace Persistency
{
namespace Postgres
{

/** \brief builder for reader of prelude format.
 */
class ObjectFactoryBuilder: public BackendFactory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a object's facotry for Persistency::Postgres.
   */
  ObjectFactoryBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const std::string  name_;
  const Logger::Node log_;
}; // class ObjectFactoryBuilder

} // namespace Postgres
} // namespace Persistency

#endif
