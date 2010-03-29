/*
 * Tree.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TREE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TREE_HPP_FILE

#include <vector>

#include "Persistency/IO/Postgres/TreePtr.hpp"
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
  /** \brief id of tree node
   */
  typedef DataBaseID IDNode;

  /** \brief create tree mode
   *  \param id       ID of tree node
   *  \param children vector of children IDs
   */
  Tree(IDNode id, const std::vector<IDNode> &children);

  /** \brief get number of children in tree node
   *  \return number of children
   */
  std::vector<IDNode>::size_type getChildrenNumber();

  /** \brief get children IDs
   *  \return vector with children IDs
   */
  const std::vector<IDNode> &getChildren();

private:
  void addChild(IDNode idChild);
  IDNode getID();

  std::vector<IDNode> children_;
  IDNode         id_;
}; // class Tree

} // namespace Postgres
} // namespace IO
} // namespace Persistency
#endif
