/*
 * Parse.cpp
 *
 */
#include <cassert>

#include "ConfigIO/Preprocessor/Parse.hpp"
#include "ConfigIO/Preprocessor/FunctionParser.hpp"

namespace ConfigIO
{
namespace Preprocessor
{
// helper namespace
namespace
{

Rule::Mode parseRuleMode(const std::string &mode)
{
  if(mode=="equals")
    return Rule::Mode::EQUALS;
  if(mode=="lessthan")
    return Rule::Mode::LESSTHAN;
  if(mode=="greaterthan")
    return Rule::Mode::GREATERTHAN;
  if(mode=="contains")
    return Rule::Mode::CONTAINS;
  if(mode=="regexp")
    return Rule::Mode::REGEXP;
  if(mode=="regexpci")
    return Rule::Mode::REGEXPCI;

  // Ooops...
  throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "unknown rule's mode: " + mode);
}

Rule parseComplexRule(const XML::Node &rule)
{
  assert( rule.getName()=="rule" );

  // check attributes
  const XML::AttributesList &attrs=rule.getAttributesList();
  int                        index=0;
  bool                       hasPath=false;
  bool                       hasMode=false;
  std::string                pathStr;
  std::string                modeStr;
  std::string                valueStr;
  std::string                formatterStr="";   // default formatter
  for(XML::AttributesList::const_iterator it=attrs.begin(); it!=attrs.end(); ++it)
  {
    // ensure only two attributes are present
    ++index;
    if(index>3)
      throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "invalid attributes count for term 'rule'");

    // check by name
    if( it->getName()=="path" )
    {
      pathStr=it->getValue();
      hasPath=true;
    }
    else
    {
      if( it->getName()=="formatter" )
      {
        formatterStr=it->getValue();
      }
      else  // this means mode
      {
        modeStr =it->getName();
        valueStr=it->getValue();
        hasMode =true;
      }
    }
  } // for(attributes)

  // is everything on place?
  if(!hasMode)
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "mode is missing for rule");
  if(!hasPath)
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "path is missing for rule");
  // NOTE: formatter is optional

  const FunctionParser funcParse(formatterStr);

  return Rule::makeRule(pathStr, parseRuleMode(modeStr), valueStr, funcParse.getConfig() );
} // parseComplexRule()


Rule parseRule(const XML::Node &rule)
{
  const std::string &name=rule.getName();

  // sanity check
  if( rule.getChildrenList().size()!=0 )
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "term " + name + "cannot have children nodes");

  // TRUE
  if(name=="true")
    return Rule::makeTrue();

  // FALSE
  if(name=="false")
    return Rule::makeFalse();

  // sanity check
  if(name!="rule")
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "unknown term (node): " + name);
  assert(name=="rule");

  // RULE
  return parseComplexRule(rule);
} // parseRule()


// forward declaration for mutual recurrsion
Expression parseExpression(const XML::Node &expression);

Expression::Expressions parseExpressionsSet(const XML::Node &parent)
{
  // each expression must have at least two elements
  if( parent.getChildrenList().size()<2 )
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "not enought children for expression type: " + parent.getName() );

  // prase each element
  Expression::Expressions expressions;
  for(XML::Node::TNodesList::const_iterator it=parent.getChildrenList().begin(); it!=parent.getChildrenList().end(); ++it)
    expressions.push_back( parseExpression(*it) );
  return expressions;
}


Expression parseExpression(const XML::Node &expression)
{
  const std::string &name=expression.getName();

  // AND
  if(name=="and")
    return Expression::makeAnd( parseExpressionsSet(expression) );

  // OR
  if(name=="or")
    return Expression::makeOr(  parseExpressionsSet(expression) );

  const XML::Node::TNodesList &children=expression.getChildrenList();

  // NOT
  if(name=="not")
  {
    // 'not' must have exactly one element
    if( children.size()!=1 )
      throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "invalid children count for expression type: not");
    assert( children.size()==1 );
    return Expression::makeNot( parseExpression( *children.begin() ) );
  }

  // assume TERM
  return Expression::makeTerm( parseRule(expression) );
} // parseExpression()


Section::Type getSectionType(const XML::Node &section)
{
  const std::string &name=section.getName();
  if(name=="reject")
    return Section::Type::REJECT;
  if(name=="accept")
    return Section::Type::ACCEPT;

  // we shouldn't be here
  throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "unknown section type: '" + name + "'");
} // getSectionType()


Section parseSection(const XML::Node &section)
{
  const Section::Type type=getSectionType(section);

  if( section.getChildrenList().size()!=1 )
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "invalid children count for given section");
  assert( section.getChildrenList().begin()!=section.getChildrenList().end() );
  const Expression expression=parseExpression( *section.getChildrenList().begin() );

  return Section(type, expression);
} // parseSection()


Config parseConfig(const XML::Node &root)
{
  Config::Sections sections;

  const XML::Node::TNodesList &children=root.getChildrenList();
  sections.reserve( children.size() );
  for(XML::Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
    sections.push_back( parseSection(*it) );

  return Config(sections);
} // parseConfig()

} // unnamed namespace




Parse::Parse(const XML::Node &root):
  cfg_( parseConfig(root) )
{
}

} // namespace Preprocessor
} // namespace ConfigIO
