/*
 * FactoryBuilder.hpp
 *
 */
#ifndef INCLUDE_FILTER_SIMILARITY_FACTORYBUILDER_HPP_FILE
#define INCLUDE_FILTER_SIMILARITY_FACTORYBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Filter/Factory.hpp"

namespace Filter
{
namespace Similarity
{

/** \brief builds same-name filter
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

} // namespace Similarity
} // namespace Filter

#endif
