/*
 * Tree.cpp
 *
 */
#include "Persistency/IO/Postgres/Tree.hpp"

using namespace std;
namespace Persistency
{
namespace IO
{
namespace Postgres
{

Tree::Tree(IDNode id, const vector<IDNode> &children):id_(id)
{
  for(vector<IDNode>::const_iterator it = children.begin(); it != children.end(); ++it)
  {
    addChild(*it);
  }
}

Tree::IDNode Tree::getID()
{
  return id_;
}

void Tree::addChild(IDNode childID)
{
  children_.push_back( childID );
}

vector<Tree::IDNode>::size_type Tree::getChildrenNumber()
{
  return children_.size();
}

const vector<Tree::IDNode> &Tree::getChildren()
{
  return children_;
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
