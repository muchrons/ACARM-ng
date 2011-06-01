/*
 * FactoryBuilder.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_EXTAPP_FACTORYBUILDER_HPP_FILE
#define INCLUDE_TRIGGER_EXTAPP_FACTORYBUILDER_HPP_FILE

#include <string>

#include "ConfigIO/TriggerConfig.hpp"
#include "Logger/Logger.hpp"
#include "Commons/Convert.hpp"
#include "Trigger/Factory.hpp"
#include "Trigger/ExceptionInvalidValue.hpp"

namespace Trigger
{
namespace ExtApp
{

/** \brief builds trigger running external applications.
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

} // namespace ExtApp
} // namespace Trigger

#endif
