/*
 * ExceptionInvalidParameter.hpp
 *
 */
#ifndef INCLUDE_FILTER_SIMILARITY_EXCEPTIONINVALIDPARAMTER_HPP_FILE
#define INCLUDE_FILTER_SIMILARITY_EXCEPTIONINVALIDPARAMTER_HPP_FILE

#include "Filter/Similarity/Exception.hpp"

namespace Filter
{
namespace Similarity
{
/** \brief exception thrown on invalid config's parameter
 */
class ExceptionInvalidParameter: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of the paramter.
   *  \param what  message to represent.
   */
  ExceptionInvalidParameter(const Location &where, const std::string &name, const std::string &what):
    Exception(where, cc("invalid value of paramter '", name, "'; ", what) )
  {
  }
}; // class ExceptionInvalidParameter

} // namespace Similarity
} // namespace Filter

#endif
