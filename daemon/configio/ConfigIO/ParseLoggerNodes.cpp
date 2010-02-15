/*
 * ParseLoggerNodes.cpp
 *
 */
#include <cassert>

#include "ConfigIO/ParseLoggerNodes.hpp"
#include "ConfigIO/OptionalString.hpp"

using namespace std;
using namespace XML;

namespace ConfigIO
{

ParseLoggerNodes::ParseLoggerNodes(const XML::Node &node):
  defaultNode_( parseDefaultNode(node) ),
  nodes_( parse(node) )
{
}


LoggerNodeConfig ParseLoggerNodes::parseDefaultNode(const XML::Node &node) const
{
  // this in fact will be already checked before this call happens
  assert(node.getName()=="nodes");

  const string &appender =node.getAttributesList().getAttribute("appender").getValue();
  const string &threshold=node.getAttributesList().getAttribute("threshold").getValue();
  return LoggerNodeConfig("", appender, threshold);
}


LoggerNodes ParseLoggerNodes::parse(const XML::Node &node) const
{
  // this in fact will be already checked before this call happens
  assert(node.getName()=="nodes");
  // parse whole tree and gather results on 'nodes' variable.
  LoggerNodes::Nodes nodes;
  addParsed(nodes, defaultNode_.getNodeName(), node, "");
  // return gathered results
  return LoggerNodes(nodes);
}


void ParseLoggerNodes::addParsed(LoggerNodes::Nodes &out,
                                 const std::string  &parent,
                                 const XML::Node    &node,
                                 const char         *sep) const
{
  assert(sep!=NULL);
  assert(string(".")==sep || *sep==0);

  const Node::TNodesList &children=node.getChildrenList();

  // iterate through all nodes
  for(Node::TNodesList::const_iterator it=children.begin();
      it!=children.end(); ++it)
  {
    const string &name=it->getName();
    const string  path=parent + sep + name;

    //
    // add this node to collection
    //
    const AttributesList           &al=it->getAttributesList();
    AttributesList::const_iterator  ait;
    OptionalString                  app;
    OptionalString                  thr;

    // check if appender is present
    ait=al.find("appender");
    if( ait!=al.end() )
      app=ait->getValue();

    // check if threshold is present
    ait=al.find("threshold");
    if( ait!=al.end() )
      thr=ait->getValue();

    // add to collection
    out.push_back( LoggerNodeConfig(path, app, thr) );

    //
    // dive into subtree(s)
    //
    addParsed(out, path, *it, ".");
  }
}

} // namespace ConfigIO
