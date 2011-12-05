/*
 * ExceptionBadNumberOfNodeChildren.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONBADNUMBEROFNODECHILDREN_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_EXCEPTIONBADNUMBEROFNODECHILDREN_HPP_FILE

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
  /** \brief create execption with given message.
   *  \param where place where exception has been rised
   *  \param id    ID of node.
   */
  ExceptionBadNumberOfNodeChildren(const Location &where, const DataBaseID &id):
    Exception(where, cc("node ", id, " has less than two children") )
  {
  }
}; // class ExceptionBadNumberOfNodeChildren

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif

