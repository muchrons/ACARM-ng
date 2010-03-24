/*
 * Tree.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TREE_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TREE_HPP_FILE

#include<vector>
#include "TreePtr.hpp"
using namespace std;

namespace Persistency
{
namespace IO
{
namespace Postgres
{

class Tree{
public:
  typedef int IDNode;
  Tree(IDNode id, vector<IDNode> children);
  void print();
  int  getChildrenNumber();
  vector<IDNode> getChildren();

private:
  void addChild(IDNode idChild);
  IDNode getID();
  vector<IDNode> children_;
  IDNode         id_;
}; // class Tree

} // namespace Postgres
} // namespace IO
} // namespace Persistency
#endif
