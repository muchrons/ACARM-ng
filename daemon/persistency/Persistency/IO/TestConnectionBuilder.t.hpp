/*
 * TestConnectionBuilder.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_TESTCONNECTIONBUILDER_T_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_TESTCONNECTIONBUILDER_T_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"

namespace Persistency
{
namespace IO
{

/** \brief builder for internal, test connection.
 */
class TestConnectionBuilder: public BackendFactory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a object's factory for Persistency's tests.
   */
  TestConnectionBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const std::string  name_;
  const Logger::Node log_;
}; // class TestConnectionBuilder

} // namespace IO
} // namespace Persistency

#endif
