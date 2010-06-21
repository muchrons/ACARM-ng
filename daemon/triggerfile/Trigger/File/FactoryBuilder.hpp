/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_FACTORYBUILDER_HPP_FILE
#define INCLUDE_TRIGGER_FILE_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Trigger/Factory.hpp"
#include "Trigger/File/ExceptionInvalidDirectory.hpp"

namespace Trigger
{
namespace File
{

/** \brief builds file report trigger.
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

} // namespace File
} // namespace Trigger

#endif
