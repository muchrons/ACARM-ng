/*
 * SaxParser.cpp
 *
 */
#include <string>
#include <string.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"

using namespace std;
using namespace XML;


namespace XML
{
namespace XMLpp
{

XML::Tree SaxParser::parse(const char *doc)
{
  finishedOk_=false;
  clear();
  assert( nodeStack_.empty() );

  // parse input page. unfortunetly there is a shity cast required... :(
  const unsigned char *udata=reinterpret_cast<const unsigned char*>(doc);
  const size_t         len  =strlen(doc);
  parse_memory_raw(udata, len);

  // check return status (in case of error exception
  // probably have been already thrown anyhow)
  if( !finishedOk_ || errorSpotted() )
    throw Exception("HTML_Extractor::XMLpp::SaxParser::parse(): "
                    "error(s) spotted during parsing");
  return *tree_;
}

void SaxParser::on_start_document()
{
  assert( nodeStack_.empty() );
  tree_=SharedTree();                   // prepare new element
}

void SaxParser::on_end_document()
{
  // check if docuemnt is not empty
  if( tree_.get()==NULL )
  {
    const string err="HTML_Extractor::XMLpp::SaxParser::on_end_document(): "
                     "no elements found in XML";
    problems_.push_back(err);
    finishedOk_     =false;
    return;
  }

  // check if document parsing did not finished too early...
  if( !nodeStack_.empty() )
  {
    const string err="HTML_Extractor::XMLpp::SaxParser::on_end_document(): "
                     "tags' enclosure problem - document ended while "
                     "some of the nodes are still open";
    problems_.push_back(err);
    finishedOk_     =false;
    return;
  }

  finishedOk_=true;
  assert( nodeStack_.empty() );
}

void SaxParser::on_start_element(const Glib::ustring& name,
                                 const AttributeList& attributes)
{
  XML::Node *child=NULL;
  // first (root) element already added?
  if( tree_.get()!=NULL )
  {
    // register to parent and add self to nodes stack
    XML::Node *parent=nodeStack_.top();   // get stack element
    assert(parent!=NULL);
    child=parent->addChild( Node(name) );
  }
  else  // no root present yet
  {
    assert( nodeStack_.empty() );
    tree_=SharedTree( new Tree( Node(name) ) );
    child=&tree_->getRoot();              // use root element here
  }
  assert(child!=NULL);
  nodeStack_.push(child);
  assert(nodeStack_.top()==child);

  // add all attributes
  for(xmlpp::SaxParser::AttributeList::const_iterator it=attributes.begin();
      it!=attributes.end();
      ++it)
  {
    const XML::Attribute attr(it->name, it->value);
    child->addAttribute(attr);
  }
}

void SaxParser::on_end_element(const Glib::ustring& name)
{
  name.c_str();         // to suppres compiler warrning
  assert( nodeStack_.top()->getName()==name );
  nodeStack_.pop();     // discard current node's pointer
}

void SaxParser::on_characters(const Glib::ustring& characters)
{
  const string  str =characters;
  XML::Node    *node=nodeStack_.top();
  node->addValue( XML::Value(str) );
}

void SaxParser::on_comment(const Glib::ustring&)
{
  // skip comments
}

void SaxParser::on_warning(const Glib::ustring& text)
{
  string err="HTML_Extractor::XMLpp::SaxParser: warrning in XML: ";
  err+=text;
  problems_.push_back(err);
}

void SaxParser::on_error(const Glib::ustring& text)
{
  string err="HTML_Extractor::XMLpp::SaxParser: error in XML: ";
  err+=text;
  problems_.push_back(err);
}

void SaxParser::on_fatal_error(const Glib::ustring& text)
{
  string err="HTML_Extractor::XMLpp::SaxParser: fatal error in XML: ";
  err+=text;
  problems_.push_back(err);
}

void SaxParser::clear(void)
{
  // assign new object in place of prevoius one
  nodeStack_=std::stack<XML::Node*>();
  // clean-up problems report
  problems_.clear();
}

} // namespace XMLpp
} // namespace XML
