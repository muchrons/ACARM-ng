/*
 * SaxParser.t.cpp
 *
 */
#include <tut.h>
#include <string.h>
#include <boost/shared_array.hpp>

#include "XML/XMLpp/SaxParser.hpp"

namespace
{
  const char *xhtml1=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE html\n"
    "PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
    "\"DTD/xhtml1-strict.dtd\">\n"
    "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"
    "<head>\n"
    "<title> Strict DTD XHTML Example </title>\n"
    "</head>\n"
    "<body>\n"
    "<p>\n"
    "Please Choose a Day:\n"
    "<br /><br />\n"
    "<select name=\"day\">\n"
    "<option selected=\"selected\">Monday</option>\n"
    "<option>Tuesday</option>\n"
    "<option>Wednesday</option>\n"
    "</select>\n"
    "</p>\n"
    "</body>\n"
    "</html>\n";

  const char *xhtml2=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE html\n"
    "PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n"
    "\"DTD/xhtml1-transitional.dtd\">\n"
    "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"
    "<head>\n"
    "<title> Transitional DTD XHTML Example </title>\n"
    "</head>\n"
    "\n"
    "<body bgcolor=\"#FFFFFF\" link=\"#000000\" text=\"red\">\n"
    "<p>This is a transitional XHTML example</p>\n"
    "</body>\n"
    "</html>\n";

  const char *xhtml3=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE html\n"
    "PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\"\n"
    "\"DTD/xhtml1-frameset.dtd\">\n"
    "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"
    "<head>\n"
    "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />""<title> Frameset DTD XHTML Example </title>\n"
    "</head>\n"
    "\n"
    "<frameset cols=\"100,*\">\n"
    "<frame src=\"toc.html\" />\n"
    "<frame src=\"intro.html\" name=\"content\" />\n"
    "</frameset>\n"
    "</html>\n";

  // missing new line...
  const char *invalid1=
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE html"                            // <---- ...here
    "PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\"\n"
    "\"DTD/xhtml1-frameset.dtd\">\n"
    "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"
    "<head>\n"
    "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />""<title> Frameset DTD XHTML Example </title>\n"
    "</head>\n"
    "\n"
    "<frameset cols=\"100,*\">\n"
    "<frame src=\"toc.html\" />\n"
    "<frame src=\"intro.html\" name=\"content\" />\n"
    "</frameset>\n"
    "</html>\n";

  // not closed tag...
  const char *invalid2=
    "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
    "<!-- Edited by XMLSpy�->\n"
    "<note>\n"
    "   <to>Tove</to>\n"
    "   <from>Jani</Ffrom>\n"                   // <---- ...here
    "   <heading>Reminder</heading>\n"
    "   <body>Don't forget me this weekend!</body>\n"
    "</note>\n";

  // not closed tag, no comment...
  const char *invalid3=
    "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
    "<note>\n"
    "   <to>Tove</to>\n"
    "   <from>Jani</Ffrom>\n"                   // <---- ...here
    "   <heading>Reminder</heading>\n"
    "   <body>Don't forget me this weekend!</body>\n"
    "</note>\n";

  // no root...
  const char *invalid4=
    "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";

  // multiple root tags...
  const char *invalid5=
    "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
    "<note>\n"
    "</note>\n"
    "<note>\n"                                  // <---- ....here
    "</note>\n";

  typedef boost::shared_array<char> TData;
}


namespace XML
{
namespace XMLpp
{

struct SaxParserTestClass
{
  TData mkData(const int no)
  {
    const char *xml=NULL;
    switch(no)
    {
      case 1:
        xml=xhtml1;
        break;
      case 2:
        xml=xhtml2;
        break;
      case 3:
        xml=xhtml3;
        break;
      default:
        assert(!"this code should be never reached - invalid number");
        break;
    }
    return mkData(xml);
  }

  TData mkInvalidData(const int no)
  {
    const char *xml=NULL;
    switch(no)
    {
      case 1:
        xml=invalid1;
        break;
      case 2:
        xml=invalid2;
        break;
      case 3:
        xml=invalid3;
        break;
      case 4:
        xml=invalid4;
        break;
      case 5:
        xml=invalid5;
        break;
      default:
        assert(!"this code should be never reached - invalid number");
        break;
    }
    return mkData(xml);
  }

  void checkThrowOnInvalidData(SaxParser &sp, int n)
  {
    TData data=mkInvalidData(n);
    try
    {
      sp.parseContent( data.get() );    // will throw on error
      tut::fail("SaxParser::parse() didn't throw on error");
    }
    catch(const std::exception&)
    {
      // this is expected
    }
  }

  TData mkData(const char *content)
  {
    assert(content!=NULL);
    const size_t len=strlen(content)+1;
    TData array(new char[len]);
    strcpy(array.get(), content);
    return array;
  }

  void validateSecondXHTML(XML::Tree tree)
  {
    using namespace tut;
    using namespace std;

    // helper typedefs
    typedef XML::Node::TNodesList      TChildList;
    typedef TChildList::const_iterator const_it;

    //
    // this is VERY long test, since there are many
    // values that have to be checked. all reading should
    // be done fine, so at least one XML must be strictly
    // checked...
    //

    // HTML
    {
      const XML::Node                       &html   =tree.getRoot();
      const XML::Node::TNodesList           &cList  =html.getChildrenList();
      XML::Node::TNodesList::const_iterator  childIt=cList.begin();
      ensure("root element is NOT html", html.getName()=="html");
      ensure("HTML does not have any children", childIt!=cList.end() );

      // check attributes
      const XML::AttributesList           &aList=html.getAttributesList();
      XML::AttributesList::const_iterator  it   =aList.begin();

      ensure("HTML attributes are missing", it!=aList.end() );
      ensure("first attribute name is invalid",
             it->getName()=="xmlns");
      ensure("first attribute values is invalid",
             it->getValue()=="http://www.w3.org/1999/xhtml");

      ++it;
      ensure("second HTML attribute is missing", it!=aList.end() );
      ensure("second attribute name is invalid",
             it->getName()=="xml:lang");
      ensure("second attribute values is invalid",
             it->getValue()=="en");

      ++it;
      ensure("third HTML attribute is missing", it!=aList.end() );
      ensure("third attribute name is invalid",
             it->getName()=="lang");
      ensure("third attribute values is invalid",
             it->getValue()=="en");

      ++it;
      ensure("too many attributes have been read", it==aList.end() );

      // check values
      const XML::Node::TValuesList &vList=html.getValuesList();
      ensure("no values have been read (should be some white chars)",
             vList.begin()!=vList.end() );

      // HEAD
      {
        const XML::Node                       &head    =*childIt;
        const XML::Node::TNodesList           &hcList  =head.getChildrenList();
        XML::Node::TNodesList::const_iterator  hchildIt=hcList.begin();
        ensure("head element not found", head.getName()=="head");

        // check attributes
        const XML::AttributesList           &haList=head.getAttributesList();
        XML::AttributesList::const_iterator  itHa  =haList.begin();

        ensure("head has some attributes", itHa==haList.end() );

        // check values
        const XML::Node::TValuesList &hvList=head.getValuesList();
        ensure("no values have been read (should be some white chars)",
               hvList.begin()!=hvList.end() );

        // TITLE
        {
          ensure("title tag not found", hchildIt!=hcList.end() );
          const XML::Node              &title=*hchildIt;
          const XML::Node::TNodesList  &tcList=title.getChildrenList();
          ensure("head element not found", title.getName()=="title");
          ensure("title has child elements", tcList.begin()==tcList.end() );

          // check attributes
          const XML::AttributesList    &taList=title.getAttributesList();
          ensure("title has some attributes", taList.begin()==taList.end() );

          // check values
          const XML::Node::TValuesList &tvList=title.getValuesList();
          ensure("no values have been read (should be some white chars)",
                 tvList.begin()!=tvList.end() );
          // look for predefined string
          bool foundStr=false;
          for(XML::Node::TValuesList::const_iterator itV=tvList.begin();
              itV!=tvList.end();
              ++itV)
          {
            const string &str=itV->get();
            if( str.find(" Transitional DTD XHTML Example ") <
                str.length() )   // got it?
            {
              foundStr=true;
              break;
            }
          }
          ensure("title string not found", foundStr);
        } // /TITLE
      } // /HEAD

      ++childIt;
      // BODY
      {
        const XML::Node                       &body    =*childIt;
        const XML::Node::TNodesList           &bcList  =body.getChildrenList();
        XML::Node::TNodesList::const_iterator  hchildIt=bcList.begin();
        ensure("body element not found", body.getName()=="body");

        // check attributes
        const XML::AttributesList           &baList=body.getAttributesList();
        XML::AttributesList::const_iterator  itAtt=baList.begin();
        ensure("body doesn't have attributes", itAtt!=baList.end() );
        ensure("first attribute name is invalid",
               itAtt->getName()=="bgcolor");
        ensure("first attribute values is invalid",
               itAtt->getValue()=="#FFFFFF");

        ++itAtt;
        ensure("second BODY attribute is missing", itAtt!=baList.end() );
        ensure("second attribute name is invalid",
               itAtt->getName()=="link");
        ensure("second attribute values is invalid",
               itAtt->getValue()=="#000000");

        ++itAtt;
        ensure("third HTML attribute is missing", itAtt!=baList.end() );
        ensure("third attribute name is invalid",
               itAtt->getName()=="text");
        ensure("third attribute values is invalid",
               itAtt->getValue()=="red");

        ++itAtt;
        ensure("too many attributes have been read", itAtt==baList.end() );

        // check values
        const XML::Node::TValuesList &bvList=body.getValuesList();
        ensure("no values have been read (should be some white chars)",
               bvList.begin()!=bvList.end() );

        // P
        {
          ensure("P tag not found", hchildIt!=bcList.end() );
          const XML::Node              &par  =*hchildIt;
          const XML::Node::TNodesList  &pcList=par.getChildrenList();
          ensure("P element not found", par.getName()=="p");
          ensure("P has child elements", pcList.begin()==pcList.end() );

          // check attributes
          const XML::AttributesList    &paList=par.getAttributesList();
          ensure("P has some attributes", paList.begin()==paList.end() );

          // check values
          const XML::Node::TValuesList &pvList=par.getValuesList();
          ensure("no values have been read (should be some white "
                 "chars and text)",
                 pvList.begin()!=pvList.end() );
          // look for predefined string
          bool foundStr=false;
          for(XML::Node::TValuesList::const_iterator itV=pvList.begin();
              itV!=pvList.end();
              ++itV)
          {
            const string &str=itV->get();
            if( str.find("This is a transitional XHTML example") <
                str.length() )   // got it?
            {
              foundStr=true;
              break;
            }
          }
          ensure("paragraph string not found", foundStr);
        } // /P
      } // /BODY

      // this should be it
      ++childIt;
      ensure("some tag after BODY found", childIt==cList.end() );
    } // /HTML

    //
    // Uff... if we're here, parsed XML looks fine! :)
    //
  }
};

} // namespace XMLpp
} // namespace XML


namespace tut
{
typedef XML::XMLpp::SaxParserTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("XML/XMLpp/SaxParser");
}

using namespace std;
using namespace XML;
using namespace XML::XMLpp;


namespace tut
{

// test constructor/destructor
template<>
template<>
void testObj::test<1>(void)
{
  SaxParser sp;
  ensure("unknown error has been repoted", !sp.errorSpotted() );
}

// test on parsing valid page 1.
template<>
template<>
void testObj::test<2>(void)
{
  SaxParser sp;
  TData     data=mkData(2);
  XML::Tree tree=sp.parseContent( data.get() ); // parse tree
  validateSecondXHTML(tree);                    // check if tree is ok
  ensure("unknown error has been repoted", !sp.errorSpotted() );
}

// try paring all available XHTMLs, but do not check output
template<>
template<>
void testObj::test<3>(void)
{
  for(int i=1; i<=3; ++i)
  {
    SaxParser sp;
    TData     data=mkData(1);
    sp.parseContent( data.get() );
    ensure("unknown error has been repoted", !sp.errorSpotted() );
  }
}

// test if parse() throws on error (1)
template<>
template<>
void testObj::test<4>(void)
{
  SaxParser sp;
  TData     data=mkInvalidData(1);
  try
  {
    sp.parseContent( data.get() );    // will throw on error
    fail("SaxParser::parseContent() didn't throw on error");
  }
  catch(const std::exception&)
  {
    // this is expected
  }
}

// try multiple parsing with one instance of parser
template<>
template<>
void testObj::test<5>(void)
{
  SaxParser sp;
  // first call
  {
    TData     data=mkData(2);
    XML::Tree tree=sp.parseContent( data.get() );  // parse tree
    validateSecondXHTML(tree);              // check if tree is ok
    ensure("unknown error has been repoted (1)", !sp.errorSpotted() );
  }
  // second call
  {
    TData data=mkData(1);
    sp.parseContent( data.get() );  // parse tree
    ensure("unknown error has been repoted (2)", !sp.errorSpotted() );
  }
  // third call
  {
    TData     data=mkData(2);
    XML::Tree tree=sp.parseContent( data.get() );  // parse tree
    validateSecondXHTML(tree);              // check if tree is ok
    ensure("unknown error has been repoted (3)", !sp.errorSpotted() );
  }
}

// check if error log is filled with data when error has been reported
template<>
template<>
void testObj::test<6>(void)
{
  SaxParser sp;
  checkThrowOnInvalidData(sp, 1);

  // check if there is something inside log
  ensure("unknown error has been repoted", sp.errorSpotted() );
  const SaxParser::TErrorList &log=sp.getErrorList();
  ensure("error log is empty", log.begin()!=log.end() );
}

// test if parse() throws on error (2)
template<>
template<>
void testObj::test<7>(void)
{
  SaxParser sp;
  checkThrowOnInvalidData(sp, 2);
}

// test if parse() throws on error (3)
template<>
template<>
void testObj::test<8>(void)
{
  SaxParser sp;
  checkThrowOnInvalidData(sp, 3);
}

// test if parse() throws on error (4)
template<>
template<>
void testObj::test<9>(void)
{
  SaxParser sp;
  checkThrowOnInvalidData(sp, 4);
}

// test if parse() throws on error (5)
template<>
template<>
void testObj::test<10>(void)
{
  SaxParser sp;
  checkThrowOnInvalidData(sp, 5);
}

} // namespace tut
