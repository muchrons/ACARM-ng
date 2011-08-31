/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PYTHON_FACTORYBUILDER_HPP_FILE
#define INCLUDE_TRIGGER_PYTHON_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Trigger/Factory.hpp"
#include "Trigger/Python/Exception.hpp"

namespace Trigger
{
namespace Python
{

/** \brief builds python script running trigger.
 */
class FactoryBuilder: public Factory::TFactoryBuilderBase
{
public:
  /** \brief creates builder's instance.
   */
  FactoryBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const Core::Types::Proc::TypeName type_;
  const Logger::Node                log_;
}; // class FactoryBuilder

} // namespace Python
} // namespace Trigger

#endif
