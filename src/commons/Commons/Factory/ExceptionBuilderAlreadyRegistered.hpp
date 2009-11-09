/*
 * ExceptionBuilderAlreadyExist.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_EXCEPTIONBUILDERALREADYEXIST_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_EXCEPTIONBUILDERALREADYEXIST_HPP_FILE

/* public header */

#include "Commons/Factory/Exception.hpp"

namespace Commons
{
namespace Factory
{

/** \brief exception when trying to re-register the same builder
 */
class ExceptionBuilderAlreadyExist: public Commons::Factory::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised.
   *  \param name  name of builder that is already registered.
   */
  explicit Exception(const char *where, const char *name)
    Commons::Factory::Exception( where,
                                 std::string("builder already registered: ") +
                                    ensureString(name) )
  {
  }
}; // class ExceptionBuilderAlreadyExist

}; // namespace Commons

#endif
