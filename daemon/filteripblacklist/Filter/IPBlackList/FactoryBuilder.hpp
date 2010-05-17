/*
 * ConnectionBuilder.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_FACTORYBUILDER_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Filter/Factory.hpp"

namespace Filter
{
namespace IPBlackList
{

/** \brief builds IP-blacklist filter
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

} // namespace IPBlackList
} // namespace Filter

#endif
