/*
 * Registrator.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_REGISTRATOR_HPP_FILE
#define INCLUDE_PLUGINS_REGISTRATOR_HPP_FILE

/* public header */

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_polymorphic.hpp>

#include "System/Plugins/DynamicObject.hpp"
#include "Plugins/ExceptionRegistrationError.hpp"

namespace Plugins
{

/** \brief registrator class, that registers given builder in a given abstract factory.
 *
 *  this class is to be directly used by the plugins, in order to register.
 */
template<typename TSingleton, typename TBuilder>
class Registrator
{
public:
  /** \brief register builder in abstract factory, holding given handle.
   *  \param dynObj handle to shared libary with a given plugin.
   */
  explicit Registrator(const System::Plugins::DynamicObject &dynObj)
  {
    registerBuilder(dynObj);
  }

private:
  /** \brief builder that holds handle to opened shared library inside.
   */
  struct DerivedBuilder: TBuilder
  {
    /** \brief constructor saving handle.
     *  \param handle handle to shared object required for given plugin.
     */
    explicit DerivedBuilder(const System::Plugins::DynamicObject &dynObj):
      dynObj_(dynObj)
    {
      // in order to safely derive from TBuilder it has to be polymorphic already.
      // it shuld be always true, since this should be interface derived object,
      // though we'd better be safe than sorry.
      BOOST_STATIC_ASSERT( boost::is_polymorphic<TBuilder>::type::value );
    }

  private:
    // holding handle ensures that library is opened as long as factory exists.
    // this is required to ensure that all symbols in use are there.
    System::Plugins::DynamicObject dynObj_;
  }; // struct DerivedBuilder


  void registerBuilder(const System::Plugins::DynamicObject &dynObj)
  {
    try
    {
      // create instance of required builder, that holds handle to shared object.
      typename TSingleton::FactoryBuilderBaseAutoPtr ptr( new DerivedBuilder(dynObj) );
      // unregister effects of automatic registration (if needed). this call does nothing
      // when builder is not yet registered.
      TSingleton::unregisterBuilder( ptr->getTypeName() );
      // now register new instance
      TSingleton::registerBuilder(ptr);
    }
    catch(const std::exception &ex)
    {
      // re-throw exception of a known type
      throw ExceptionRegistrationError(SYSTEM_SAVE_LOCATION, typeid(TBuilder).name(), ex.what() );
    }
  }
}; // class Registrator

} // namespace Plugins

#endif
