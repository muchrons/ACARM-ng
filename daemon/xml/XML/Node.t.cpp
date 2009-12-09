/*
 * Node.t.cpp
 *
 */
#include <tut.h>

#include "XML/Node.hpp"


namespace XML
{

struct NodeTestClass
{
  NodeTestClass(void):
    n_("NodeName")
  {
  }

  Node n_;
};

} // namespace XML


namespace tut
{
typedef XML::NodeTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("XML/Node");
}

using namespace std;
using namespace XML;


namespace tut
{

// test if constructed object is empty.
template<>
template<>
void testObj::test<1>(void)
{
  ensure("node name does not match delcared one", n_.getName()=="NodeName");

  const Node::TNodesList  &nl=n_.getChildrenList();
  ensure("children list is not empty",   nl.begin()==nl.end() );

  const AttributesList    &al=n_.getAttributesList();
  ensure("attributes list is not empty", al.begin()==al.end() );

  const Node::TValuesList &vl=n_.getValuesList();
  ensure("values list is not empty",     vl.begin()==vl.end() );
}

// test adding/getting children.
template<>
template<>
void testObj::test<2>(void)
{
  Node n1("Child1");
  Node n2("child2");
  n_.addChild(n1);
  n_.addChild(n2);

  const Node::TNodesList &nl=n_.getChildrenList();

  Node::TNodesList::const_iterator it=nl.begin();
  ensure("no element added", it!=nl.end() );
  ensure("invalid element has been returned (1)", it->getName()=="Child1");

  ++it;
  ensure("too little elements added", it!=nl.end() );
  ensure("invalid element has been returned (2)", it->getName()=="child2");

  ++it;
  ensure("too many elements in collection", it==nl.end() );
}

// test addition and getting of attributes.
template<>
template<>
void testObj::test<3>(void)
{
  Attribute attr1("a1", "v1");
  Attribute attr2("a2", "v2");
  n_.addAttribute(attr1);
  n_.addAttribute(attr2);

  const AttributesList &al=n_.getAttributesList();
  AttributesList::const_iterator it=al.begin();
  ensure("no element added", it!=al.end() );
  ensure("invalid element has been returned (1)", it->getName()=="a1");

  ++it;
  ensure("too little elements added", it!=al.end() );
  ensure("invalid element has been returned (2)", it->getName()=="a2");

  ++it;
  ensure("too many elements in collection", it==al.end() );
}

// values list addition and getting.
template<>
template<>
void testObj::test<4>(void)
{
  Value v1("alice");
  Value v2("cat");
  n_.addValue(v1);
  n_.addValue(v2);

  const Node::TValuesList &vl=n_.getValuesList();
  Node::TValuesList::const_iterator it=vl.begin();
  ensure("no element added", it!=vl.end() );
  ensure("invalid element has been returned (1)", it->get()=="alice");

  ++it;
  ensure("too little elements added", it!=vl.end() );
  ensure("invalid element has been returned (2)", it->get()=="cat"  );

  ++it;
  ensure("too many elements in collection", it==vl.end() );
}

// test adding/getting children of children.
template<>
template<>
void testObj::test<5>(void)
{
  Node  n1("Child1");
  Node  n2("child2");
  Node *c1=n_.addChild(n1);
  n_.addChild(n2);

  // add second level of tree
  Node c1c1("level2/c1");
  Node c1c2("level2/c2");
  c1->addChild(c1c1);
  c1->addChild(c1c2);

  // try going throught whole tree:
  {
    const Node::TNodesList           &nl=n_.getChildrenList();
    Node::TNodesList::const_iterator  it=nl.begin();

    ensure("no element added", it!=nl.end() );
    ensure("invalid element has been returned (1)", it->getName()=="Child1");
    // go deeper into Child1's children:
    {
      const Node::TNodesList           &nlc1=it->getChildrenList();
      Node::TNodesList::const_iterator  it1 =nlc1.begin();

      ensure("no element added to 'Child1'", it1!=nlc1.end() );
      ensure("invalid element has been returned for 'Child1' (1)",
             it1->getName()=="level2/c1");
      ++it1;
      ensure("too little elements added to 'Child1'", it1!=nlc1.end() );
      ensure("invalid element has been returned for 'Child1' (2)",
             it1->getName()=="level2/c2");
      ++it1;
      ensure("too many elements in collection", it1==nlc1.end() );
    }

    // back to the parrent:
    ++it;
    ensure("too little elements added", it!=nl.end() );
    ensure("invalid element has been returned (2)", it->getName()=="child2");

    ++it;
    ensure("too many elements in collection", it==nl.end() );
  }
}

// test if getChild() works
template<>
template<>
void testObj::test<6>(void)
{
  Node  n1("Child1");
  Node  n2("child2");
  n_.addChild(n1);
  n_.addChild(n2);

  ensure_equals("incorrect node found as first one",
                n_.getChild( n1.getName() ).getName(), n1.getName() );
  ensure_equals("incorrect node found as second one",
                n_.getChild( n2.getName() ).getName(), n2.getName() );
}

// test if getChild() throws on non-existing element
template<>
template<>
void testObj::test<7>(void)
{
  Node  n1("Child1");
  Node  n2("child2");
  n_.addChild(n1);
  n_.addChild(n2);

  try
  {
    n_.getChild("Child");   // no such name - should throw
    fail("getChild() didn't throw on node not found");
  }
  catch(const XML::Exception &ex)
  {
    // this is expected
  }
}

// test concatenation of values into one string
template<>
template<>
void testObj::test<8>(void)
{
  n_.addValue( Value("alice") );
  n_.addValue( Value(" ")     );
  n_.addValue( Value("has")   );
  n_.addValue( Value(" ")     );
  n_.addValue( Value("a")     );
  n_.addValue( Value(" ")     );
  n_.addValue( Value("cat")   );
  const string str=n_.getValuesString();
  ensure_equals("invalid string returned", str, "alice has a cat");
}

// test looking for empty collection range.
template<>
template<>
void testObj::test<9>(void)
{
  Node  n1("Child1");
  Node  n2("child2");
  n_.addChild(n1);
  n_.addChild(n2);

  const Node::TNodesList           &cl=n_.getChildrenList();
  Node::TNodesList::const_iterator  it=n_.getChild("child2", cl.end() );
  ensure("element found in empty search range", it==cl.end() );
}

// test looking for element with getChild() and iterator.
template<>
template<>
void testObj::test<10>(void)
{
  Node  nA ("a");
  Node  nB1("b");
  Node  nC ("c");
  Node  nB2("b");
  Node  nD ("d");
  // add elements and remember two interesting instances.
           n_.addChild(nA );
  Node *p1=n_.addChild(nB1);
           n_.addChild(nC );
  Node *p2=n_.addChild(nB2);
           n_.addChild(nD );

  const Node::TNodesList           &cl =n_.getChildrenList();
  Node::TNodesList::const_iterator  tmp=cl.begin();

  // check first element
  Node::TNodesList::const_iterator    it1=n_.getChild("b", tmp);
  ensure("first instance not found",  it1!=cl.end() );
  ensure_equals("invalid element returned as first one",
                it1->getName(), "b");
  ensure("first element's address doesn't match", p1==&(*it1) );
  tmp=it1;

  // check second element
  ++tmp;
  Node::TNodesList::const_iterator    it2=n_.getChild("b", tmp);
  ensure("second instance not found", it2!=cl.end() );
  ensure_equals("invalid element returned as second one",
                it2->getName(), "b");
  ensure("second element's address doesn't match", p2==&(*it2) );
  tmp=it2;

  // there should be no next element
  ++tmp;
  Node::TNodesList::const_iterator    it3=n_.getChild("b", tmp);
  ensure("more instances seems to be present", it3==cl.end() );
}

// test looking for non-existing element.
template<>
template<>
void testObj::test<11>(void)
{
  Node  n1("Child1");
  Node  n2("child2");
  n_.addChild(n1);
  n_.addChild(n2);

  const Node::TNodesList           &cl=n_.getChildrenList();
  Node::TNodesList::const_iterator  it=
                    n_.getChild("non-existing", cl.begin() );
  ensure("non-existing element found", it==cl.end() );
}

} // namespace tut
