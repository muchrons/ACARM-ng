/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_FACTORY_MANYTOMANY_FACTORYBUILDER_HPP_FILE
#define INCLUDE_FACTORY_MANYTOMANY_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Filter/Factory.hpp"

namespace Filter
{
namespace ManyToMany
{

/** \brief builds many-to-many filter
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

} // namespace ManyToMany
} // namespace Filter

#endif
