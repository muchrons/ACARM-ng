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

// TODO: implement
// TODO: test
// TODO: comment
// TODO: this functionality must be available as a singleton<TFactory>

namespace Commons
{
namespace Factory
{
namespace detail
{

template<typename TFactory>
class AbstractFactory
{
public:
  typedef TFactory                                      FactoryType;
  typedef FactoryBuilderBase<TFactory>                  TFactoryBuilderBase;
  typedef std::auto_ptr<TFactoryBuilderBase>            FactoryBuilderBaseAutoPtr;
  typedef typename TFactoryBuilderBase::FactoryTypeName FactoryTypeName;
  typedef typename TFactoryBuilderBase::FactoryPtr      FactoryPtr;

  typedef typename TFactoryBuilderBase::Parameter       Parameter;
  typedef typename TFactoryBuilderBase::Value           Value;
  typedef typename TFactoryBuilderBase::Options         Options;

private:
  typedef boost::shared_ptr<TFactoryBuilderBase>           FactoryBuilderBasePtr;
  typedef std::map<FactoryTypeName, FactoryBuilderBasePtr> BuildersMap;

public:


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
