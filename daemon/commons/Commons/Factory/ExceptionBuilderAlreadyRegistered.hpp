/*
 * ExceptionBuilderAlreadyRegistered.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_EXCEPTIONBUILDERALREADYREGISTRED_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_EXCEPTIONBUILDERALREADYREGISTRED_HPP_FILE

/* public header */

#include "Commons/Factory/Exception.hpp"

namespace Commons
{
namespace Factory
{

/** \brief exception when trying to re-register the same builder
 */
class ExceptionBuilderAlreadyRegistered: public Commons::Factory::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised.
   *  \param name  name of builder that is already registered.
   */
  ExceptionBuilderAlreadyRegistered(const Location &where, const char *name):
    Commons::Factory::Exception( where,
                                 std::string("builder already registered: ") +
                                    ensureString(name) )
  {
  }
}; // class ExceptionBuilderAlreadyRegistered

} // namespace Factory
} // namespace Commons

#endif
