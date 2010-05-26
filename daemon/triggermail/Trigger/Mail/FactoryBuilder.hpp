/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_FACTORYBUILDER_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Trigger/Factory.hpp"
#include "Trigger/Mail/ExceptionInvalidConfig.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief builds e-mail notifier trigger.
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

  const std::string  name_;
  const Logger::Node log_;
}; // class FactoryBuilder

} // namespace Mail
} // namespace Trigger

#endif
