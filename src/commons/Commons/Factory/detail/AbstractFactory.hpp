/*
 * AbstractFactory.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_DETAIL_ABSTRACTFACTORY_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_DETAIL_ABSTRACTFACTORY_HPP_FILE

/* public header */

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/Factory/FactoryBuilderBase.hpp"
#include "Commons/Factory/ExceptionBuilderDoesNotExist.hpp"
#include "Commons/Factory/ExceptionBuilderAlreadyRegistered.hpp"
#include "Commons/Factory/ExceptionNullBuilder.hpp"

namespace Commons
{
namespace Factory
{
namespace detail
{

/** \brief abstract factory of factories of a given type.
 */
template<typename TFactory>
class AbstractFactory: private boost::noncopyable
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
  /** \brief creates abstract factory.
   */
  AbstractFactory(void):
    log_("commons.factory.detail")
  {
    LOGMSG_INFO(log_, "created abstract factory");
  }

  /** \brief creates factory with a proper builder, and with proper options.
   *  \param name    parameter that decides which builder is to be used.
   *  \param options paramters to be builder.
   *  \return factory created.
   */
  FactoryPtr create(const FactoryTypeName &name, const Options &options) const
  {
    LOGMSG_INFO_S(log_)<<"building factory type: '"<<name<<"'";
    // check if proper builders exist
    typename BuildersMap::const_iterator cit=builders_.find(name);
    if( cit==builders_.end() )
      throw ExceptionBuilderDoesNotExist(SYSTEM_SAVE_LOCATION, name.c_str() );

    // use available builder
    assert( cit->second.get()!=NULL );
    FactoryPtr ptr=cit->second->build(options);
    assert( ptr.get()!=NULL );
    LOGMSG_DEBUG_S(log_)<<"object created with factory '"<<name<<"'";
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
      throw ExceptionNullBuilder(SYSTEM_SAVE_LOCATION);

    LOGMSG_INFO_S(log_)<<"registering factory type: '"<<ptr->getTypeName()<<"'";

    // ensure entry does not already exist
    typename BuildersMap::const_iterator cit=builders_.find( ptr->getTypeName() );
    if( cit!=builders_.end() )
      throw ExceptionBuilderAlreadyRegistered(SYSTEM_SAVE_LOCATION,
                                              ptr->getTypeName().c_str() );

    // ok - we can register it
    builders_[ptr->getTypeName()]=ptr;
    LOGMSG_DEBUG_S(log_)<<"factory type '"<<ptr->getTypeName()<<"' registered";
  }

  /** \brief unregisters given builder.
   *  \param name name of a builder to unregister.
   */
  void unregisterBuilder(const FactoryTypeName &name)
  {
    LOGMSG_INFO_S(log_)<<"unregistering factory type: '"<<name<<"'";
    // check if it is registered - if not, we're done
    typename BuildersMap::iterator it=builders_.find(name);
    if( it==builders_.end() )
    {
      LOGMSG_DEBUG_S(log_)<<"factory type '"<<name<<"' is not registered";
      return;
    }

    // unregister entry
    builders_.erase(it);
    assert( builders_.find(name)==builders_.end() );
    LOGMSG_DEBUG_S(log_)<<"factory type '"<<name<<"' unregistered";
  }

private:
  Logger::Node log_;
  BuildersMap  builders_;
}; // public AbstractFactory

} // namespace detail
} // namespace Factory
} // namespace Commons

#endif
