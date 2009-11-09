/*
 * ExceptionBuilderDoesNotExist.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FACTORY_EXCEPTIONBUILDERDOESNOTEXIST_HPP_FILE
#define INCLUDE_COMMONS_FACTORY_EXCEPTIONBUILDERDOESNOTEXIST_HPP_FILE

/* public header */

#include "Commons/Factory/Exception.hpp"

namespace Commons
{
namespace Factory
{

/** \brief exception when builder of requested name does not exist.
 */
class ExceptionBuilderDoesNotExist: public Commons::Factory::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception ha been rised.
   *  \param name  name of non-existing builder.
   */
  ExceptionBuilderDoesNotExist(const char *where, const char *name):
    Commons::Factory::Exception( where,
                                 std::string("no such builder: ") +
                                    ensureString(name) )
  {
  }
}; // class ExceptionBuilderDoesNotExist

} // namespace Factory
} // namespace Commons

#endif
