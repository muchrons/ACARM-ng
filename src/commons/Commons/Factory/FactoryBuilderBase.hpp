/*
 * FactoryBuilderBase.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_FACTORYBUILDERBASE_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_FACTORYBUILDERBASE_HPP_FILE

/* public header */

#include <string>
#include <map>
#include <memory>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/Factory/Exception.hpp"


namespace Commons
{
namespace Factory
{

/** \brief builder creating requested factories.
 */
template<typename TFactory>
class FactoryBuilderBase
{
public:
  /** \brief name of the factory type.
   */
  typedef std::string                 FactoryTypeName;
  /** \brief smart pointer to the factory instance.
   */
  typedef std::auto_ptr<TFactory>     FactoryPtr;
  /** \brief type representing parameter name.
   */
  typedef std::string                 Parameter;
  /** \brief type representing string value.
   */
  typedef std::string                 Value;
  /** \brief options map (paramter=>value).
   */
  typedef std::map<Parameter, Value>  Options;

  /** \brief creates factory builder base.
   */
  FactoryBuilderBase(void):
    log_("commons.factory")
  {
  }

  /** \brief declare virtual d-tor in base class.
   */
  virtual ~FactoryBuilderBase(void)
  {
  }

  /** \brief builds required factory, with a given options.
   *  \param options options to be passed to the new instance.
   *  \return new factory instance.
   */
  FactoryPtr build(const Options &options) const
  {
    LOGMSG_INFO_S(log_)<<"building: '"<<getTypeNameImpl()<<"'";
    FactoryPtr ptr=buildImpl(options);
    assert( ptr.get()!=NULL );
    return ptr;
  }

  /** \brief gets name of type it can build.
   *  \return name of factory it can build.
   */
  const FactoryTypeName &getTypeName(void) const
  {
    return getTypeNameImpl();
  }

private:
  virtual FactoryPtr buildImpl(const Options &options) const = 0;
  virtual const FactoryTypeName &getTypeNameImpl(void) const = 0;

  Logger::Node log_;
}; // public AbstractFactory

} // namespace Factory
} // namespace Commons

#endif
