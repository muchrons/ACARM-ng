/*
 * AttributesList.hpp
 *
 */
#ifndef INCLUDE_XML_ATTRIBUTESLIST_HPP_FILE
#define INCLUDE_XML_ATTRIBUTESLIST_HPP_FILE

/* public header */

#include <list>

#include "XML/Attribute.hpp"
#include "XML/Exception.hpp"


namespace XML
{

/** \brief class representing list of attributes assigned to
 *         given node. attributes have to be uniq.
 */
class AttributesList
{
public:
  /** \brief helper typedef for internal usage.
   */
  typedef std::list<Attribute>  TList;
  /** \brief const_iterator typedef for compatibility with STL.
   */
  typedef TList::const_iterator const_iterator;

  /**
   * \return returns iterator to begin of the collection.
   */
  const_iterator begin(void) const
  {
    return list_.begin();
  }
  /**
   * \return returns iterator to collections end.
   */
  const_iterator end(void) const
  {
    return list_.end();
  }

  /** \brief adds given attribute to collection. if attribute
   *   with the same name already exists method will throw.
   *  \param attr attribute to be added.
   */
  void add(const Attribute &attr);
  /** \brief search for a given element by its name.
   *  \param name name of attribute being searched for.
   *  \return iterator pointing to element found or end.
   */
  const_iterator find(const std::string &name) const;
  /** \brief gets attribute of a given name or throws on error.
   *  \param name name of an attribute.
   *  \return attribute with a given name.
   */
  const Attribute &getAttribute(const std::string &name) const;
private:
  TList list_;
}; // class AttributesList

} // namespace XML

#endif

