/*
 * Tree.hpp
 *
 * representation of XML tree.
 *
 */
#ifndef INCLUDE_XML_TREE_HPP_FILE
#define INCLUDE_XML_TREE_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>

#include "XML/Node.hpp"


namespace XML
{

/**
 * XML Tree representation.
 *
 * \note class is copyable with shared_ptr-like semantics.
 */
class Tree
{
public:
  /**
   * creates object with initial root element.
   * \param root root node.
   */
  explicit Tree(const Node &root):
    root_( new Node(root) )
  {
  }
  /**
   * returns reference to root element.
   * \return root element's reference.
   */
  Node &getRoot(void)
  {
    return *root_.get();
  }
  /**
   * returns read-only reference to root element.
   * \return const reference to root element.
   */
  const Node &getRoot(void) const
  {
    return *root_.get();
  }

private:
  boost::shared_ptr<Node> root_;
}; // class Tree

} // namespace XML

#endif

