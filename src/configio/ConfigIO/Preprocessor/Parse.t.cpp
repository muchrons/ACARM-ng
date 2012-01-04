/*
 * Parse.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/Preprocessor/Parse.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{

const char *xmlEmpty=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor/>"
  "</acarm_ng>"
  "";

const char *xmlNoPreprocessor=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "</acarm_ng>"
  "";

const char *xmlTermInSection=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <true/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlMultipleSections=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <true/>"
  "    </accept>"
  "    <reject>"
  "      <false/>"
  "    </reject>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleEquals=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" equals=\"alice has a cat\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleContains=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" contains=\"alice has a cat\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionOr=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <or>"
  "        <true/>"
  "        <false/>"
  "      </or>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionAnd=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <and>"
  "        <true/>"
  "        <false/>"
  "      </and>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionNot=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <not>"
  "        <true/>"
  "      </not>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionNotWithoutChildren=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <not/>"    // <-- not must have children
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionOrWithoutChildren=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <or/>"     // <-- or must have children
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionAndWithoutChildren=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <and/>"    // <-- and must have children
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermTrueWithChildren=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <true>"
  "        <true/>" // <-- true can't have children
  "      </true>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionAndWithoutOneChild=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <and>"
  "        <true/>" // <-- too few arguments for AND
  "      </and>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleWithoutPath=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule pathXYZ=\"a.b.c\" contains=\"alice has a cat\"/>"    // <-- no 'path'
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleWithoutValidMode=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" abc=\"alice has a cat\"/>"            // <-- invalid 'mode'
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRulePathAndModeSwapped=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule contains=\"alice has a cat\" path=\"a.b.c\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionNested=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <and>"
  "        <or>"
  "          <true/>"
  "          <true/>"
  "        </or>"
  "        <false/>"
  "      </and>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlExpressionAndWithOver2Children=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <and>"
  "        <true/>"
  "        <false/>"
  "        <true/>"
  "      </and>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleRegExp=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" regexp=\"gr+y\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleRegExpCI=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" regexpci=\"Gr+Y\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleLessThan=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" lessthan=\"43\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleGreaterThan=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" greaterthan=\"41\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";

const char *xmlTermRuleWithFormatter=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" formatter=\"add(value(),`2`)\" equals=\"alice has a cat\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";



struct TestClass
{
  // return copyied, parsed subtree
  XML::Node getNodes(const char *xml) const
  {
    assert(xml!=NULL);
    XML::XMLpp::SaxParser sp;
    const XML::Tree       tree=sp.parseContent(xml);
    return tree.getRoot().getChild("preprocessor");
  }

  // return copyied appenders configs
  Config getConfig(const char *xml) const
  {
    const XML::Node n=getNodes(xml);
    const Parse     ppc(n);
    return ppc.getConfig();
  }

  void testThrow(const char *xml, const char *message) const
  {
    try
    {
      getConfig(xml);       // should throw
      tut::fail(message);
    }
    catch(const ConfigIO::ExceptionParseError &)
    {
      // this is expected
    }
    catch(const XML::Exception &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/Parse");
} // unnamed namespace


namespace tut
{

// self-test
template<>
template<>
void testObj::test<1>(void)
{
  const XML::Node n=getNodes(xmlEmpty);
  ensure_equals("invalid node returned by helper", n.getName(), "preprocessor");
}

// test throw on no section
template<>
template<>
void testObj::test<2>(void)
{
  testThrow(xmlNoPreprocessor, "parser didn't thorow on lack of preprocessor's section");
}

// test section with term
template<>
template<>
void testObj::test<3>(void)
{
  const Config      c=getConfig(xmlTermInSection);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE_TRUE);
}

// test multiple sections
template<>
template<>
void testObj::test<4>(void)
{
  const Config c=getConfig(xmlMultipleSections);
  ensure_equals("invalid number of sections", c.getSections().size(), 2u);
  // test section 1
  {
    const Section    &s=c.getSections().at(0);
    ensure("invalid section 1 type", s.getType()==Section::Type::ACCEPT);
    const Expression &e=s.getExpression();
    ensure("invalid expression 1 type", e.getType()==Expression::Type::TERM);
    const Rule       &r=e.getRules().at(0);
    ensure("invalid rule 1 type", r.getType()==Rule::Type::RULE_TRUE);
  }
  // test section 2
  {
    const Section    &s=c.getSections().at(1);
    ensure("invalid section 2 type", s.getType()==Section::Type::REJECT);
    const Expression &e=s.getExpression();
    ensure("invalid expression 2 type", e.getType()==Expression::Type::TERM);
    const Rule       &r=e.getRules().at(0);
    ensure("invalid rule 2 type", r.getType()==Rule::Type::RULE_FALSE);
  }
}

// test term-rule-equals:
template<>
template<>
void testObj::test<5>(void)
{
  const Config      c=getConfig(xmlTermRuleEquals);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::EQUALS);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "alice has a cat");
}

// test term-rule-contains
template<>
template<>
void testObj::test<6>(void)
{
  const Config      c=getConfig(xmlTermRuleContains);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::CONTAINS);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "alice has a cat");
}

// test expression-or
template<>
template<>
void testObj::test<7>(void)
{
  const Config      c=getConfig(xmlExpressionOr);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::OR);
  ensure_equals("invalid OR expressions count", e.getExpressions().size(), 2u);
  // test expression 1
  {
    const Expression &tmp=e.getExpressions().at(0);
    ensure("invalid expression 1 type", tmp.getType()==Expression::Type::TERM);
  }
  // test expression 2
  {
    const Expression &tmp=e.getExpressions().at(1);
    ensure("invalid expression 2 type", tmp.getType()==Expression::Type::TERM);
  }
}

// test expression-and
template<>
template<>
void testObj::test<8>(void)
{
  const Config      c=getConfig(xmlExpressionAnd);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::AND);
  ensure_equals("invalid OR expressions count", e.getExpressions().size(), 2u);
  // test expression 1
  {
    const Expression &tmp=e.getExpressions().at(0);
    ensure("invalid expression 1 type", tmp.getType()==Expression::Type::TERM);
  }
  // test expression 2
  {
    const Expression &tmp=e.getExpressions().at(1);
    ensure("invalid expression 2 type", tmp.getType()==Expression::Type::TERM);
  }
}

// test expression-not
template<>
template<>
void testObj::test<9>(void)
{
  const Config      c=getConfig(xmlExpressionNot);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::NOT);
  ensure_equals("invalid OR expressions count", e.getExpressions().size(), 1u);
  // test expression
  const Expression &tmp=e.getExpressions().at(0);
  ensure("invalid expression type", tmp.getType()==Expression::Type::TERM);
}

// test 'not' without children
template<>
template<>
void testObj::test<10>(void)
{
  testThrow(xmlExpressionNotWithoutChildren,
            "parse didn't throw on no children for 'not'");
}

// test 'or' without childrent
template<>
template<>
void testObj::test<11>(void)
{
  testThrow(xmlExpressionOrWithoutChildren,
            "parse didn't throw on no children for 'or'");
}

// test 'and' without children
template<>
template<>
void testObj::test<12>(void)
{
  testThrow(xmlExpressionAndWithoutChildren,
            "parse didn't throw on no children for 'and'");
}

// test throw on 'true' withc children
template<>
template<>
void testObj::test<13>(void)
{
  testThrow(xmlTermTrueWithChildren,
            "parse didn't throw on children for 'true'");
}

// test throw on single child given for AND
template<>
template<>
void testObj::test<14>(void)
{
  testThrow(xmlExpressionAndWithoutOneChild, "parse didn't throw on 1 child");
}

// test throw on invalid path given for term-rule
template<>
template<>
void testObj::test<15>(void)
{
  testThrow(xmlTermRuleWithoutPath, "parse didn't throw on no path for rule");
}

// test throw when invalid mode is given for rule
template<>
template<>
void testObj::test<16>(void)
{
  testThrow(xmlTermRuleWithoutValidMode, "parse didn't throw on invalid rule mode");
}

// test if parsing is valid when 'path' and 'mode' are swaped in config
template<>
template<>
void testObj::test<17>(void)
{
  const Config      c=getConfig(xmlTermRulePathAndModeSwapped);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::CONTAINS);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "alice has a cat");
}

// test nested expressions
template<>
template<>
void testObj::test<18>(void)
{
  const Config      c=getConfig(xmlExpressionNested);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::AND);
  ensure_equals("invalid arguments count for root 'and'", e.getExpressions().size(), 2u);
  // test 'or' argument
  {
    const Expression &eTmp=e.getExpressions().at(0);
    ensure("invalid expression 1", eTmp.getType()==Expression::Type::OR);
    ensure_equals("invalid children count", eTmp.getExpressions().size(), 2u);
    // test rule 1
    {
      const Expression &te=eTmp.getExpressions().at(0);
      ensure("invalid term/expression 1", te.getType()==Expression::Type::TERM);
    }
    // test rule 2
    {
      const Expression &te=eTmp.getExpressions().at(1);
      ensure("invalid term/expression 2", te.getType()==Expression::Type::TERM);
    }
  }
  // test 'true' term
  {
    const Expression &eTmp=e.getExpressions().at(1);
    ensure("invalid term 2", eTmp.getType()==Expression::Type::TERM);
  }
}

// test args>2 for 'and'
template<>
template<>
void testObj::test<19>(void)
{
  const Config      c=getConfig(xmlExpressionAndWithOver2Children);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::AND);
  ensure_equals("invalid OR expressions count", e.getExpressions().size(), 3u);
  // test expression 1
  {
    const Expression &tmp=e.getExpressions().at(0);
    ensure("invalid expression 1 type", tmp.getType()==Expression::Type::TERM);
  }
  // test expression 2
  {
    const Expression &tmp=e.getExpressions().at(1);
    ensure("invalid expression 2 type", tmp.getType()==Expression::Type::TERM);
  }
  // test expression 3
  {
    const Expression &tmp=e.getExpressions().at(2);
    ensure("invalid expression 3 type", tmp.getType()==Expression::Type::TERM);
  }
}

// test term-rule-regexp
template<>
template<>
void testObj::test<20>(void)
{
  const Config      c=getConfig(xmlTermRuleRegExp);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::REGEXP);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "gr+y");
}

// test term-rule-regexp-case-insensitive
template<>
template<>
void testObj::test<21>(void)
{
  const Config      c=getConfig(xmlTermRuleRegExpCI);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::REGEXPCI);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "Gr+Y");
}

// test term-rule-lessthan
template<>
template<>
void testObj::test<22>(void)
{
  const Config      c=getConfig(xmlTermRuleLessThan);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::LESSTHAN);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "43");
}

// test term-rule-greaterthan
template<>
template<>
void testObj::test<23>(void)
{
  const Config      c=getConfig(xmlTermRuleGreaterThan);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  ensure("invalid section type", s.getType()==Section::Type::ACCEPT);
  const Expression &e=s.getExpression();
  ensure("invalid expression type", e.getType()==Expression::Type::TERM);
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::GREATERTHAN);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "41");
}

// test rule with non-default formatter
template<>
template<>
void testObj::test<24>(void)
{
  const Config           c=getConfig(xmlTermRuleWithFormatter);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section         &s=c.getSections().at(0);
  const Expression      &e=s.getExpression();
  const Rule            &r=e.getRules().at(0);
  const FormatterConfig &f=r.getFormatter();
  ensure("invalid formatter's root", f.get().isFunction() );
  ensure_equals("invalid arguments count", f.get().argCount(), 2);
  ensure("invalid first argument", f.get().param(0).isValue() );
  ensure("invalid second argument", f.get().param(1).isArgument() );
  ensure_equals("invalid second argument's value", f.get().param(1).argument(), "2");
}


namespace
{
const char *xmlTermRuleInNum=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[41]\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<25>(void)
{
  const Config      c=getConfig(xmlTermRuleInNum);
  ensure_equals("invalid number of sections", c.getSections().size(), 1u);
  const Section    &s=c.getSections().at(0);
  const Expression &e=s.getExpression();
  const Rule       &r=e.getRules().at(0);
  ensure("invalid rule type", r.getType()==Rule::Type::RULE);
  ensure("invalid rule mode", r.getMode()==Rule::Mode::IN);
  ensure_equals("invalid path", r.getPath(), "a.b.c");
  ensure_equals("invalid value", r.getValue(), "alice has a cat");
}

namespace
{
const char *xmlTermRuleInStrNum=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[`42`]\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<26>(void)
{
}

namespace
{
const char *xmlTermRuleInMultipleElems=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[`abc`,`def`,123]\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<27>(void)
{
}

namespace
{
const char *xmlTermRuleInEmptySet=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[]\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<28>(void)
{
}

namespace
{
const char *xmlTermRuleInSetSpaces=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[`a b` , `x` , 123]\"/>"
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<29>(void)
{
}

namespace
{
const char *xmlTermRuleInNoStartBracket=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"123]\"/>"    // <-- no start bracket
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<30>(void)
{
}

namespace
{
const char *xmlTermRuleInNoEndBracket=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[123\"/>"    // <-- no end bracket
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<31>(void)
{
}

namespace
{
const char *xmlTermRuleInNoBrackets=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"123\"/>"     // <-- no brackets
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<32>(void)
{
}

namespace
{
const char *xmlTermRuleInUnterminatedQuote=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[`123]\"/>"  // <-- unterminated quote
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<33>(void)
{
}

namespace
{
const char *xmlTermRuleInInvalidNumber=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  "  <preprocessor>"
  "    <accept>"
  "      <rule path=\"a.b.c\" in=\"[1 23]\"/>"  // <-- invalid number
  "    </accept>"
  "  </preprocessor>"
  "</acarm_ng>"
  "";
} // unnamed namespace

//
template<>
template<>
void testObj::test<34>(void)
{
}

} // namespace tut
