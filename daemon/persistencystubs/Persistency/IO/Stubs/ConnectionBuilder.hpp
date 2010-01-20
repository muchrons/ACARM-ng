/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_CONNECTIONBUILDER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_CONNECTIONBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief builder for connection stubs.
 */
class ConnectionBuilder: public BackendFactory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a object's facotry for Persistency::Stubs.
   */
  ConnectionBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const std::string  name_;
  const Logger::Node log_;
}; // class ConnectionBuilder

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
