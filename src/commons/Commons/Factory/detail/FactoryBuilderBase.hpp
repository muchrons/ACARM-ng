/*
 * FactoryBuilderBase.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_DETAIL_FACTORYBUILDERBASE_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_DETAIL_FACTORYBUILDERBASE_HPP_FILE

/* public header */

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "Commons/Factory/Exception.hpp"

// TODO: comment

namespace Commons
{
namespace Factory
{
namespace detail
{

template<typename TFactory>
class FactoryBuilderBase
{
public:
  typedef std::string                 FactoryTypeName;
  typedef boost::shared_ptr<TFactory> FactoryPtr;

  typedef std::string                 Parameter;
  typedef std::string                 Value;
  typedef std::map<Parameter, Value>  Options;

  virtual ~FactoryBuilderBase(void)
  {
  }

  virtual FactoryPtr build(const Options &options) const = 0;

  virtual const FactoryTypeName &getTypeName(void) const = 0;
}; // public AbstractFactory

} // namespace detail
} // namespace Factory
} // namespace Commons

#endif
