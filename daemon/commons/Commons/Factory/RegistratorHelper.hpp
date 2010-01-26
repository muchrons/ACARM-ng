/*
 * RegistratorHelper.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_REGISTRATORHELPER_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_REGISTRATORHELPER_HPP_FILE

/* public header */

#include <sstream>
#include <iostream>

#include "BuildProcess/ForceLink.hpp"
#include "Logger/Logger.hpp"

namespace Commons
{
namespace Factory
{

/** \brief registers given builder to given singleton factory.
 *
 *  \note this object is usefull only for registration before main(), since
 *        it does not throw any errors. instead bool value is used.
 */
template<typename TSingleton, typename TBuilder>
class RegistratorHelper
{
public:
  /** \brief register builder in singleton.
   *  \note this c-tor newer throws.
   */
  RegistratorHelper(void):
    registered_( registerBuilder() )
  {
  }

  /** \brief checks if registration was successfull.
   *  \return true if registration succeeded, false otherwise.
   */
  bool isRegistered(void) const
  {
    return registered_;
  }

private:
  // following code loks a bit strange but it has to be assured that no
  // exception is thrown from it. on the other hand, if possible it
  // tries to put builder's beging registered in logs to make eventual
  // debugging easier.
  bool registerBuilder(void) const
  {
    try
    {

      // initialize basic helper objects
      const Logger::Node                             node("commons.factory");
      typename TSingleton::FactoryBuilderBaseAutoPtr ptr(new TBuilder);

      // try register given builder
      try
      {
        LOGMSG_INFO(node, ( "registering builder: " +
                            ptr->getTypeName() ).c_str() );
        TSingleton::registerBuilder(ptr);
      }
      catch(const std::exception &ex)
      {
        const std::string msg="exception cought while registering: " +
                              ptr->getTypeName() + ": " + ex.what();
        LOGMSG_FATAL(node, msg.c_str() );
        return false;
      }
      catch(...)
      {
        const std::string msg="unknown exception while registering: " +
                              ptr->getTypeName();
        LOGMSG_FATAL(node, msg.c_str() );
        return false;
      } // try{}catch() for registration

    }
    catch(const std::exception &ex)
    {
      std::cerr<<"RegistrationHelper: error while registering factory: "
               << ex.what() << std::endl;
      return false;
    }
    catch(...)
    {
      std::cerr<<"RegistrationHelper: unknown error while registering factory"
               << std::endl;
      return false;
    } // try{}catch() before registration

    return true;
  }

  // notice that making instance of this object a global object takes adventage
  // of C/C++ feature, that each global, before initialized is zeroerd. in
  // practice this means that registered_ will be '0' (i.e. 'false') and
  // so isRegistered(), when not registered, will return proper value, even
  // though object has not been created yet. memory access is valid too, since
  // it uses part of memeory assigned by the compiler during build time, to
  // be exclusive for that particular instance.
  bool registered_;
}; // class RegistratorHelper

} // namespace Factory
} // namespace Commons

#endif
