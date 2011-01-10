/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_FACTORYBUILDER_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Trigger/Factory.hpp"
#include "Trigger/ExceptionInvalidValue.hpp"
#include "Commons/Convert.hpp"
#include "ConfigIO/TriggerConfig.hpp"

namespace Trigger
{
namespace Jabber
{

/** \brief builds Jabber notifier trigger.
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

  const std::string  type_;
  const Logger::Node log_;
}; // class FactoryBuilder

} // namespace Jabber
} // namespace Trigger

#endif
