/*
 * AbstractFactory.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_DETAIL_ABSTRACTFACTORY_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_DETAIL_ABSTRACTFACTORY_HPP_FILE

/* public header */

#include <map>
#include <boost/shared_ptr.hpp>
#include <cassert>

#include "Commons/Factory/detail/FactoryBuilderBase.hpp"
#include "Commons/Factory/ExceptionBuilderDoesNotExist.hpp"
#include "Commons/Factory/ExceptionBuilderAlreadyRegistered.hpp"
#include "Commons/Factory/ExceptionNullBuilder.hpp"

// TODO: this functionality must be available as a singleton<TFactory>

namespace Commons
{
namespace Factory
{
namespace detail
{

/** \brief abstract factory of factories of a given type.
 */
template<typename TFactory>
class AbstractFactory
{
public:
  /** \brief template paramter forward.
   */
  typedef TFactory                                      FactoryType;
  /** \brief base builder type for a given factory type.
   */
  typedef FactoryBuilderBase<TFactory>                  TFactoryBuilderBase;
  /** \brief auto pointer to factor builder's base type.
   */
  typedef std::auto_ptr<TFactoryBuilderBase>            FactoryBuilderBaseAutoPtr;
  /** \brief forward factory anme type parameter.
   */
  typedef typename TFactoryBuilderBase::FactoryTypeName FactoryTypeName;
  /** \brief forward factory pointer paramter.
   */
  typedef typename TFactoryBuilderBase::FactoryPtr      FactoryPtr;
  /** \brief forward parameter type.
   */
  typedef typename TFactoryBuilderBase::Parameter       Parameter;
  /** \brief forward value type.
   */
  typedef typename TFactoryBuilderBase::Value           Value;
  /** \brief forward options type declaration.
   */
  typedef typename TFactoryBuilderBase::Options         Options;

private:
  typedef boost::shared_ptr<TFactoryBuilderBase>           FactoryBuilderBasePtr;
  typedef std::map<FactoryTypeName, FactoryBuilderBasePtr> BuildersMap;

public:
  /** \brief creates factory with a proper builder, and with proper options.
   *  \param name    parameter that decides which builder is to be used.
   *  \param options paramters to be builder.
   *  \return factory created.
   */
  FactoryPtr create(const FactoryTypeName &name, const Options &options) const
  {
    // check if proper builders exist
    typename BuildersMap::const_iterator cit=builders_.find(name);
    if( cit==builders_.end() )
      throw ExceptionBuilderDoesNotExist(CALLNAME, name.c_str() );

    // use available builder
    assert( cit->second.get()!=NULL );
    const FactoryPtr ptr=cit->second->build(options);
    assert( ptr.get()!=NULL );
    return ptr;
  }

  /** \brief registers new builder.
   *  \param fb builder to be registered.
   */
  void registerBuilder(FactoryBuilderBaseAutoPtr fb)
  {
    FactoryBuilderBasePtr ptr( fb.release() );  // transform auto_ptr<> to shared_ptr<>
    // check if pointer is valid
    if( ptr.get()==NULL )
      throw ExceptionNullBuilder(CALLNAME);

    // ensure entry does not already exist
    typename BuildersMap::const_iterator cit=builders_.find( ptr->getTypeName() );
    if( cit!=builders_.end() )
      throw ExceptionBuilderAlreadyRegistered(CALLNAME,
                                              ptr->getTypeName().c_str() );

    // ok - we can register it
    builders_[ptr->getTypeName()]=ptr;
  }

  /** \brief unregisters given builder.
   *  \param name name of a builder to unregister.
   */
  void unregisterBuilder(const FactoryTypeName &name)
  {
    // check if it is registered - if not, we're done
    typename BuildersMap::iterator it=builders_.find(name);
    if( it==builders_.end() )
      return;

    // unregister entry
    builders_.erase(it);
    assert( builders_.find(name)==builders_.end() );
  }

private:
  BuildersMap builders_;
}; // public AbstractFactory

} // namespace detail
} // namespace Factory
} // namespace Commons

#endif
