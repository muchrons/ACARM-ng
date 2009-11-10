/*
 * RegistratorHelper.cpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_REGISTRATORHELPER_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_REGISTRATORHELPER_HPP_FILE

#include <sstream>

#include "Logger/Logger.hpp"

#include "Input/Prelude/ReaderBuilder.hpp"
#include "Input/Prelude/Reader.hpp"
#include "Input/ReadersFactory.hpp"

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
    catch(...)
    {
      return false;
    } // try{}catch() before registration

    return true;
  }

  bool registered_;
}; // class RegistratorHelper

} // namespace Factory
} // namespace Commons

#endif
