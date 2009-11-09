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
#include "Commons/Factory/ExceptionNullBuilder.hpp"

// TODO: implement
// TODO: test
// TODO: comment
// TODO: this code must be available as a singleton

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
  typedef TFactory                     TFactory;
  typedef FactoryBuilderBase<TFactory> TFactoryBuilderBase;

private:
  typedef boost::shared_ptr<TFactoryBuilderBase>           FactoryBuilderBasePtr;
  typedef std::map<FactoryTypeName, FactoryBuilderBasePtr> BuildersMap;

public:
  typedef std::auto_ptr<TFactoryBuilderBase>   TFactoryBuilderBaseAutoPtr;
  typedef TFactoryBuilderBase::FactoryTypeName FactoryTypeName;
  typedef TFactoryBuilderBase::FactoryPtr      FactoryPtr;

  typedef TFactoryBuilderBase::Parameter       Parameter;
  typedef TFactoryBuilderBase::Value           Value;
  typedef TFactoryBuilderBase::Options         Options;

  FactoryPtr create(const FactoryTypeName &name, const Options &options) const
  {
    BuildersMap::const_iterator cit=builders_.find(name);
    if( cit==builders_.end() )
      throw ExceptionBuilderDoesNotExist(CALLNAME, name.c_str() );

    assert( cit->get()!=NULL );
    const FactoryPtr ptr=(*cit)->build(options);
    assert( ptr.get()!=NULL );
    return ptr;
  }

  void registerFactory(FactoryBuilderBaseAutoPtr fb)
  {
    FactoryBuilderBasePtr ptr( rb.release() );  // transform auto_ptr<> to shared_ptr<>
    // check if pointer is valid
    if( ptr.get()==NULL )
      throw ExceptionNullBuilder(CALLNAME);
    // ensure entry does not already exist
    BuildersMap::const_iterator cit=builders_.find( ptr->getTypeName() );
    if( cit==builders_.end() )
      throw ExceptionBuilderAlreadyRegistered(CALLNAME, ptr->getTypeName() );
    // ok - we can register it
    builders_[ptr->getTypeName()]=ptr;
  }

  void unregisterFactory(const FactoryTypeName &name)
  {
    // check if it is registered - if not, we're done
    BuildersMap::iterator it=builders_.find(name);
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
