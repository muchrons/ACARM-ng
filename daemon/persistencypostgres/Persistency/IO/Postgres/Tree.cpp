#include "Tree.hpp"

Tree::Tree(IDNode id, vector<IDNode> children):id_(id)
{
  for(vector<IDNode>::iterator it = children.begin(); it != children.end(); ++it)
  {
    addChild(*it);
  }
}

int Tree::getID()
{
  return id_;
}

void Tree::addChild(IDNode childID)
{
  children_.push_back( childID );
}

void Tree::print()
{
  for(vector<IDNode>::iterator it = children_.begin(); it != children_.end(); ++it)
  {
    cout << "Node id: " << id_ << " Child id: " << (*it) << endl;
  }
}

int Tree::getChildrenNumber()
{
  return children_.size();
}

vector<int> Tree::getChildren()
{
  return children_;
}

