/*
 * Tree.cpp
 *
 */
#include "Persistency/IO/Postgres/Tree.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

Tree::Tree(IDNode id, vector<IDNode> children):id_(id)
{
  for(vector<IDNode>::iterator it = children.begin(); it != children.end(); ++it)
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

// TODO: remove dead code
// TODO: note - in future, if such a code is needed better write test or mtest
//       to perform this - print() should not be part of Tree class (see:
//       single responsability principle).
/*
void Tree::print()
{
  for(vector<IDNode>::iterator it = children_.begin(); it != children_.end(); ++it)
  {
    cout << "Node id: " << id_ << " Child id: " << (*it) << endl;
  }
}
*/

int Tree::getChildrenNumber()
{
  return children_.size();
}

vector<Tree::IDNode> Tree::getChildren()
{
  return children_;
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
