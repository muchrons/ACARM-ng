/*
 * Node.hpp
 *
 */
#ifndef INCLUDE_XML_NODE_HPP_FILE
#define INCLUDE_XML_NODE_HPP_FILE

/* public header */

#include <list>
#include <string>

#include "XML/AttributesList.hpp"
#include "XML/Value.hpp"
#include "XML/Exception.hpp"


namespace XML
{

/**
 * representation of single node from XML tree.
 */
class Node
{
public:
  /**
   * list type holding child nodes.
   */
  typedef std::list<Node>  TNodesList;
  /**
   * list type holding values.
   */
  typedef std::list<Value> TValuesList;

  /**
   * creates new node.
   * \param name name of current node.
   */
  explicit Node(const std::string &name):
    name_(name)
  {
  }

  /**
   * gets name of the node element.
   * \return name of a node.
   */
  const std::string getName(void) const
  {
    return name_;
  }
  /**
   * adds new child to current parrent and returns
   * reference to newly added element.
   * \param node node to be added.
   * \return reference to newly added node.
   */
  Node *addChild(const Node &node)
  {
    children_.push_back(node);
    return &( *children_.rbegin() );
  }
  /**
   * gives read-only access to children nodes.
   * \return const reference to children list.
   */
  const TNodesList &getChildrenList(void) const
  {
    return children_;
  }
  /**
   * looks for child given by name in children collection.
   * if more than one with the same name is present, the first
   * one is returned.
   * \note throws if child does not exist.
   * \param name name of child to be looked for.
   * \return reference to child node being searched.
   */
  const Node &getChild(const std::string &name) const;
  /**
   * looks for child given by name, starting from
   * given iterator. if element is not found, end-iterator
   * is return. it can be used to loop through elements
   * of the same name, if more than one is present.
   * \param  name name of child to look for.
   * \param  from iterator to place where to start.
   * \return const iterator pointing to element found or
   *         end-iterator if not found.
   */
  Node::TNodesList::const_iterator getChild(
                const std::string                &name,
                Node::TNodesList::const_iterator  from) const;

  /**
   * inserts new attribute to attributes list.
   * \param attr attribute to be added.
   */
  void addAttribute(const Attribute &attr)
  {
    attribs_.add(attr);
  }
  /**
   * gives read-only access to list of attributes.
   * \return const reference to list of attributes.
   */
  const AttributesList &getAttributesList(void) const
  {
    return attribs_;
  }
  /**
   * adds new value to values list.
   * \param val value to be added.
   */
  void addValue(const Value &val)
  {
    values_.push_back(val);
  }
  /**
   * gives read-only access to list of values.
   * \return const reference to values list.
   */
  const TValuesList& getValuesList(void) const
  {
    return values_;
  }
  /**
   * concatenates all values and returns them as single string.
   * \return concatenation of all values.
   */
  std::string getValuesString(void) const;

private:
  std::string    name_;
  AttributesList attribs_;
  TValuesList    values_;
  TNodesList     children_;
}; // class Node

} // namespace XML

#endif

