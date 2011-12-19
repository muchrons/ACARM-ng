/*
 * Section.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Singleton.hpp"
#include "Preprocessor/Section.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace ConfigIO::Preprocessor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
  TestClass(void):
    alert_( makeNewLeaf() )
  {
  }

  const Persistency::ConstGraphNodePtrNN alert_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Section");
} // unnamed namespace


namespace tut
{

// test createing default reject-section
template<>
template<>
void testObj::test<1>(void)
{
  const Rule                  r=Rule::makeTrue();
  const Expression            e=Expression::makeTerm(r);
  const Section               s(Section::Type::REJECT, e);
  const Preprocessor::Section ps(s);
  ensure("reject section didn't worked", ps.process(alert_)==Preprocessor::Section::Decision::REJECT );
}

// test createing default accept-section
template<>
template<>
void testObj::test<2>(void)
{
  const Rule                  r=Rule::makeTrue();
  const Expression            e=Expression::makeTerm(r);
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("accept section didn't worked", ps.process(alert_)==Preprocessor::Section::Decision::ACCEPT );
}

// negative test for reject-section
template<>
template<>
void testObj::test<3>(void)
{
  const Rule                  r=Rule::makeFalse();
  const Expression            e=Expression::makeTerm(r);
  const Section               s(Section::Type::REJECT, e);
  const Preprocessor::Section ps(s);
  ensure("reject section didn't worked", ps.process(alert_)==Preprocessor::Section::Decision::CONTINUE );
}

// negative test for accept-section
template<>
template<>
void testObj::test<4>(void)
{
  const Rule                  r=Rule::makeFalse();
  const Expression            e=Expression::makeTerm(r);
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("accept section didn't worked", ps.process(alert_)==Preprocessor::Section::Decision::CONTINUE );
}

// test computing random rule
template<>
template<>
void testObj::test<5>(void)
{
  // create sample formatter
  ConfigIO::Singleton::get()->rereadConfig("testdata/formatters_cfg/no_formatter.xml");
  const ConfigIO::Preprocessor::Section &sTmp=ConfigIO::Singleton::get()->preprocessorConfig().getSections().at(0);
  const FormatterConfig                 &fc=sTmp.getExpression().getRules().at(0).getFormatter();
  // test itself
  const Rule                  r =Rule::makeRule("metaalert.alert.name", Rule::Mode::EQUALS, "some alert", fc);
  const Expression            e =Expression::makeTerm(r);
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("Rule creation failed", ps.process(alert_)==Preprocessor::Section::Decision::ACCEPT );
}

// test OR structure - positive test
template<>
template<>
void testObj::test<6>(void)
{
  const Rule                  r1=Rule::makeTrue();
  const Rule                  r2=Rule::makeFalse();
  Expression::Expressions     es;
  es.push_back( Expression::makeTerm(r1) );
  es.push_back( Expression::makeTerm(r2) );
  const Expression            e=Expression::makeOr(es);
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("OR positive test failed", ps.process(alert_)==Preprocessor::Section::Decision::ACCEPT );
}

// test OR structure - negative test
template<>
template<>
void testObj::test<7>(void)
{
  const Rule                  r1=Rule::makeFalse();
  const Rule                  r2=Rule::makeFalse();
  Expression::Expressions     es;
  es.push_back( Expression::makeTerm(r1) );
  es.push_back( Expression::makeTerm(r2) );
  const Expression            e=Expression::makeOr(es);
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("OR negative test failed", ps.process(alert_)==Preprocessor::Section::Decision::CONTINUE );
}

// test AND strucutre - positive test
template<>
template<>
void testObj::test<8>(void)
{
  const Rule                  r1=Rule::makeTrue();
  const Rule                  r2=Rule::makeTrue();
  Expression::Expressions     es;
  es.push_back( Expression::makeTerm(r1) );
  es.push_back( Expression::makeTerm(r2) );
  const Expression            e=Expression::makeAnd(es);
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("AND positive test failed", ps.process(alert_)==Preprocessor::Section::Decision::ACCEPT );
}

// test AND structure - negative test
template<>
template<>
void testObj::test<9>(void)
{
  const Rule                  r1=Rule::makeTrue();
  const Rule                  r2=Rule::makeTrue();
  Expression::Expressions     es;
  es.push_back( Expression::makeTerm(r1) );
  es.push_back( Expression::makeTerm(r2) );
  const Expression            e=Expression::makeAnd(es);
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("AND positive test failed", ps.process(alert_)==Preprocessor::Section::Decision::ACCEPT );
}

// test NOT structure - positive test
template<>
template<>
void testObj::test<10>(void)
{
  const Rule                  r=Rule::makeFalse();
  const Expression            e=Expression::makeNot( Expression::makeTerm(r) );
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("NOT positive test failed", ps.process(alert_)==Preprocessor::Section::Decision::ACCEPT );
}

// test NOT structure - negative test
template<>
template<>
void testObj::test<11>(void)
{
  const Rule                  r=Rule::makeTrue();
  const Expression            e=Expression::makeNot( Expression::makeTerm(r) );
  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("NOT negative test failed", ps.process(alert_)==Preprocessor::Section::Decision::CONTINUE );
}

// test nested and/or/not (multi-level structure)
template<>
template<>
void testObj::test<12>(void)
{
  // r1 && r2
  const Rule                  r1=Rule::makeTrue();
  const Rule                  r2=Rule::makeTrue();
  Expression::Expressions     es1;
  es1.push_back( Expression::makeTerm(r1) );
  es1.push_back( Expression::makeTerm(r2) );
  const Expression            e1=Expression::makeAnd(es1);

  // r3 || r4
  const Rule                  r3=Rule::makeFalse();
  const Rule                  r4=Rule::makeTrue();
  Expression::Expressions     es2;
  es2.push_back( Expression::makeTerm(r3) );
  es2.push_back( Expression::makeTerm(r4) );
  const Expression            e2=Expression::makeOr(es2);

  // !r5
  const Rule                  r5=Rule::makeFalse();
  const Expression            e3=Expression::makeNot( Expression::makeTerm(r5) );

  // es1 && es2 && e3
  Expression::Expressions     es3;
  es3.push_back(e1);
  es3.push_back(e2);
  es3.push_back(e3);
  const Expression            e=Expression::makeAnd(es3);

  const Section               s(Section::Type::ACCEPT, e);
  const Preprocessor::Section ps(s);
  ensure("AND positive test failed", ps.process(alert_)==Preprocessor::Section::Decision::ACCEPT );
}

} // namespace tut
