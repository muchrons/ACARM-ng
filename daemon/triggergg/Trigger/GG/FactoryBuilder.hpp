/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_FACTORYBUILDER_HPP_FILE
#define INCLUDE_TRIGGER_GG_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Trigger/Factory.hpp"

namespace Trigger
{
namespace GG
{

/** \brief builds Gadu-Gadu notifier trigger.
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

} // namespace GG
} // namespace Trigger

#endif
