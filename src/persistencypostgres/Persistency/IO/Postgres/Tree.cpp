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

Tree::Tree(DataBaseID id, const std::vector<DataBaseID> &children):
  id_(id)
{
  for(vector<DataBaseID>::const_iterator it = children.begin();
      it != children.end(); ++it)
    addChild(*it);
}

DataBaseID Tree::getID()
{
  return id_;
}

void Tree::addChild(DataBaseID childID)
{
  children_.push_back( childID );
}

vector<DataBaseID>::size_type Tree::getChildrenNumber()
{
  return children_.size();
}

const vector<DataBaseID> &Tree::getChildren()
{
  return children_;
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
