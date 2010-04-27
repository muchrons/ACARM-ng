/*
 * ExceptionBadNumberOfNodeChildren.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONBADNUMBEROFNODECHILDREN_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONBADNUMBEROFNODECHILDREN_HPP_FILE

/* public header */

#include "Persistency/IO/Postgres/Exception.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
/** \brief exception thorw when SQL query returns no entries.
 */
class ExceptionBadNumberOfNodeChildren: public Exception
{
public:
  // TODO: 'id' should be taken as DataBaseID, not string - it is exception's responsability
  //       to properly format message.
  /** \brief create execption with given message.
   *  \param where place where exception has been rised
   *  \param id    ID of node.
   */
  ExceptionBadNumberOfNodeChildren(const Location &where, const std::string &id):
    // TODO: use cc() protected methods for output creation
    Exception(where, std::string("node ") + id + std::string(" has less than two children") )
  {
  }
}; // class ExceptionBadNumberOfNodeChildren

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif

