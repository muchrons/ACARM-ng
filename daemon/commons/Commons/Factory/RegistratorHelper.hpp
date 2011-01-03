/*
 * RegistratorHelper.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_REGISTRATORHELPER_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_REGISTRATORHELPER_HPP_FILE

/* public header */

#include <sstream>
#include <utility>
#include <string>
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
private:
  typedef std::pair<bool, std::string> RegistrationMark;

public:
  /** \brief register builder in singleton.
   *  \note this c-tor newer throws.
   */
  RegistratorHelper(void):
    registered_( registerBuilder() )
  {
  }
  /** \brief unregisters builder from singleton.
   */
  ~RegistratorHelper(void)
  {
    if( isRegistered() )
      TSingleton::unregisterBuilder(registered_.second);
    registered_.first=false;
  }

  /** \brief checks if registration was successfull.
   *  \return true if registration succeeded, false otherwise.
   */
  bool isRegistered(void) const
  {
    return registered_.first;
  }

private:
  // following code loks a bit strange but it has to be assured that no
  // exception is thrown from it. on the other hand, if possible it
  // tries to put builder's beging registered in logs to make eventual
  // debugging easier.
  RegistrationMark registerBuilder(void) const
  {
    try
    {

      // initialize helper objects
      const Logger::Node node("commons.factory");
      std::string        builderName=typeid(TBuilder).name();

      // try register given builder
      try
      {
        typename TSingleton::FactoryBuilderBaseAutoPtr ptr(new TBuilder);
        const std::string typeName=ptr->getTypeName();
        builderName=ptr->getTypeName() + " (" + builderName + ")";
        LOGMSG_INFO_S(node)<<"registering builder: "<<builderName;
        TSingleton::registerBuilder(ptr);
        return RegistrationMark(true, typeName);    // everything's fine
      }
      catch(const std::exception &ex)
      {
        LOGMSG_FATAL_S(node) << "exception caught while registering: "
                             << builderName << ": " << ex.what();
      }
      catch(...)
      {
        LOGMSG_FATAL_S(node) << "unknown exception while registering: "
                             << builderName;
      } // try{}catch() for registration

    }
    catch(const std::exception &ex)
    {
      std::cerr<<"RegistrationHelper: error while registering factory: "
               <<"unable to create logger node: " << ex.what() << std::endl;
    }
    catch(...)
    {
      std::cerr<<"RegistrationHelper: unknown error while registering factory: "
               <<"unable to create logger node" << std::endl;
    } // try{}catch() before registration

    return RegistrationMark(false, "");             // some error occured
  }

  // notice that making instance of this object a global object takes adventage
  // of C/C++ feature, that each global, before initialized is zeroerd. in
  // practice this means that registered_ will be '0' (i.e. 'false') and
  // so isRegistered(), when not registered, will return proper value, even
  // though object has not been created yet. memory access is valid too, since
  // it uses part of memeory assigned by the compiler during build time, to
  // be exclusive for that particular instance.
  // this is a note for bool paramter - string will be filled, when needed.
  RegistrationMark registered_;
}; // class RegistratorHelper

} // namespace Factory
} // namespace Commons

#endif
