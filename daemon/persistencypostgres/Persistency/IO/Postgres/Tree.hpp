/*
 * Tree.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TREE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TREE_HPP_FILE

#include <vector>

#include "Persistency/IO/Postgres/DataBaseID.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief tree which represents connections in data stored in data base
 */
class Tree
{
public:
  /** \brief collection of IDs read from DB. */
  typedef std::vector<DataBaseID>       IDsVector;
  /** \brief create tree mode
   *  \param id       ID of tree node
   *  \param children vector of children IDs
   */
  Tree(DataBaseID id, const std::vector<DataBaseID> &children);

  /** \brief get number of children in tree node
   *  \return number of children
   */
  std::vector<DataBaseID>::size_type getChildrenNumber();

  /** \brief get children IDs
   *  \return vector with children IDs
   */
  const std::vector<DataBaseID> &getChildren();

private:
  void addChild(DataBaseID idChild);
  DataBaseID getID();

  std::vector<DataBaseID> children_;
  DataBaseID              id_;
}; // class Tree


/** \brief shared pointer to Tree instance. */
typedef boost::shared_ptr<Tree> TreePtr;

} // namespace Postgres
} // namespace IO
} // namespace Persistency
#endif
