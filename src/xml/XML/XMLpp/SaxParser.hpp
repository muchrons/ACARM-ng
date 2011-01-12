/*
 * SaxParser.hpp
 *
 * implementation of XML++'s SaxParser building syntax
 * tree in terms of XML::Tree class.
 */
#ifndef INCLUDE_XML_XMLPP_SAXPARSER_HPP_FILE
#define INCLUDE_XML_XMLPP_SAXPARSER_HPP_FILE

/* public header */

#include <list>
#include <stack>
#include <string>
#include <libxml++/libxml++.h>

#include "XML/Tree.hpp"
#include "XML/Exception.hpp"

namespace XML
{
namespace XMLpp
{

/**
 * listener wrapper for XML++ sax parser.
 */
class SaxParser: private xmlpp::SaxParser
{
public:
  /** \brief type of error list.
   */
  typedef std::list<std::string> TErrorList;
  /**
   * performs whole parsing from XHTML to XML tree.
   * \param  doc document to be parsed.
   * \return XML tree of a parsed page.
   */
  XML::Tree parseContent(const char *doc);
  /**
   * checks if there have been an error during last
   * call of parse().
   * \note in case of error parse will throw as well.
   * \return true if last call of parse() returned error.
   */
  bool errorSpotted(void) const
  {
    return !problems_.empty();
  }
  /**
   * returns read-only reference to error log from
   * last call of parse().
   * \return const reference to error log.
   */
  const TErrorList &getErrorList(void) const
  {
    return problems_;
  }

private:
  //
  // listener calls derived from XML++
  //
  virtual void on_start_document();
  virtual void on_end_document();
  virtual void on_start_element(const Glib::ustring& name,
                                const AttributeList& attributes);
  virtual void on_end_element(const Glib::ustring& name);
  virtual void on_characters(const Glib::ustring& characters);
  virtual void on_comment(const Glib::ustring& text);
  virtual void on_warning(const Glib::ustring& text);
  virtual void on_error(const Glib::ustring& text);
  virtual void on_fatal_error(const Glib::ustring& text);

  /**
   * clears all internal collections.
   */
  void clear(void);

  typedef boost::shared_ptr<XML::Tree> SharedTree;
  SharedTree             tree_;
  bool                   finishedOk_;
  std::stack<XML::Node*> nodeStack_;
  TErrorList             problems_;
}; // class SaxParser

} // namespace XMLpp
} // namespace XML

#endif
