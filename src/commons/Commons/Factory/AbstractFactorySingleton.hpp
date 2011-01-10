/*
 * AbstractFactorySingleton.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_ABSTRACTFACTORY_SINGLETON_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_ABSTRACTFACTORY_SINGLETON_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <cassert>

#include "System/Singleton.hpp"
#include "System/OneInstanceAtOnce.hpp"
#include "Commons/Factory/detail/AbstractFactory.hpp"


namespace Commons
{
namespace Factory
{

/** \brief abstract factory of factories of a given type.
 */
template<typename TFactory>
class AbstractFactorySingleton: private System::OneInstanceAtOnce< AbstractFactorySingleton<TFactory> >
{
private:
  // implementaiton's short name
  typedef detail::AbstractFactory<TFactory>                    TAbstractFactory;
  // singleton's short name
  typedef System::Singleton<TAbstractFactory>                  SingletonImpl;

public:
  /** \brief template paramter forward.
   */
  typedef TFactory                                             FactoryType;
  /** \brief base builder type for a given factory type.
   */
  typedef typename TAbstractFactory::TFactoryBuilderBase       TFactoryBuilderBase;
  /** \brief auto pointer to factor builder's base type.
   */
  typedef typename TAbstractFactory::FactoryBuilderBaseAutoPtr FactoryBuilderBaseAutoPtr;
  /** \brief forward factory anme type parameter.
   */
  typedef typename TAbstractFactory::FactoryTypeName           FactoryTypeName;
  /** \brief forward factory pointer paramter.
   */
  typedef typename TAbstractFactory::FactoryPtr                FactoryPtr;
  /** \brief forward parameter type.
   */
  typedef typename TAbstractFactory::Parameter                 Parameter;
  /** \brief forward value type.
   */
  typedef typename TAbstractFactory::Value                     Value;
  /** \brief forward options type declaration.
   */
  typedef typename TAbstractFactory::Options                   Options;

  /** \brief creates factory with a proper builder, and with proper options.
   *  \param name    parameter that decides which builder is to be used.
   *  \param options paramters to be builder.
   *  \return factory created.
   */
  static FactoryPtr create(const FactoryTypeName &name, const Options &options)
  {
    assert( SingletonImpl::get()!=NULL );
    return SingletonImpl::get()->create(name, options);
  }

  /** \brief registers new builder.
   *  \param fb builder to be registered.
   */
  static void registerBuilder(FactoryBuilderBaseAutoPtr fb)
  {
    assert( SingletonImpl::get()!=NULL );
    SingletonImpl::get()->registerBuilder(fb);
  }

  /** \brief unregisters given builder.
   *  \param name name of a builder to unregister.
   */
  static void unregisterBuilder(const FactoryTypeName &name)
  {
    assert( SingletonImpl::get()!=NULL );
    SingletonImpl::get()->unregisterBuilder(name);
  }

private:
  // no instances allowed
  AbstractFactorySingleton(void);
}; // public AbstractFactorySingleton

} // namespace Factory
} // namespace Commons

#endif
