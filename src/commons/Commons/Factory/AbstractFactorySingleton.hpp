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
#include "Commons/Factory/detail/AbstractFactory.hpp"

// TODO: this functionality must be available as a singleton<TFactory>

namespace Commons
{
namespace Factory
{

/** \brief abstract factory of factories of a given type.
 */
template<typename TFactory>
class AbstractFactorySingletonImpl: private boost::noncopyable
{
private:
  /** \brief implementation short name.
   */
  typedef detail::AbstractFactory<TFactory>                    TAbstractFactory;

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

public:
  /** \brief creates factory with a proper builder, and with proper options.
   *  \param name    parameter that decides which builder is to be used.
   *  \param options paramters to be builder.
   *  \return factory created.
   */
  FactoryPtr create(const FactoryTypeName &name, const Options &options) const
  {
    return factory_.create(name, options);
  }

  /** \brief registers new builder.
   *  \param fb builder to be registered.
   */
  void registerBuilder(FactoryBuilderBaseAutoPtr fb)
  {
    factory_.registerBuilder(fb);
  }

  /** \brief unregisters given builder.
   *  \param name name of a builder to unregister.
   */
  void unregisterBuilder(const FactoryTypeName &name)
  {
    factory_.unregisterBuilder(name);
  }

private:
  // this prevents user from creating instances
  friend class System::Singleton< AbstractFactorySingletonImpl<TFactory> >;
  AbstractFactorySingletonImpl(void)
  {
  }

  detail::AbstractFactory<TFactory> factory_;
}; // public AbstractFactorySingletonImpl


/** \brief singleton giving access to AbstractFactory instance, for a given
 *         factory type.
 */
template<typename TFactory>
struct AbstractFactorySingleton:
  public System::Singleton< AbstractFactorySingletonImpl<TFactory> >
{
  typedef AbstractFactorySingletonImpl<TFactory> IMPL;
}; // struct AbstractFactorySingleton

} // namespace Factory
} // namespace Commons

#endif
