/*
 * TreePtr.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TREEPTR_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TREEPTR_HPP_FILE

#include<boost/shared_ptr.hpp>
namespace Persistency
{
namespace IO
{
namespace Postgres
{
// forward declaration, needed for smart-pointer decalration
class Tree;

typedef boost::shared_ptr<Tree> TreePtr;

} // namespace Postgres
} // namespace IO
} // namespace Persistency
#endif
