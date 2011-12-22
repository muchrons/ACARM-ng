/*
 * FunctionParser.cpp
 *
 */
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>

#include "ConfigIO/Preprocessor/FunctionParser.hpp"

namespace qi     =boost::spirit::qi;
namespace ascii  =boost::spirit::ascii;
namespace phoenix=boost::phoenix;

using namespace std;


typedef ConfigIO::Preprocessor::detail::FormatterConfigData Data;


// tell boost::fusion about data structure to be used
BOOST_FUSION_ADAPT_STRUCT(
      Data,
      (Data::Type,      type_)  // at_c<0>
      (std::string,     str_ )  // at_c<1>
      (Data::Arguments, args_)  // at_c<2>
    )

namespace ConfigIO
{
namespace Preprocessor
{

namespace
{

struct ErrorReporter
{
  template <typename, typename, typename>
  struct result { typedef void type; };

  template <typename Iterator>
  void operator()(qi::info const &what, Iterator errPos, Iterator last) const
  {
    stringstream ss;
    ss<<"parse error near '"<<string(errPos, last)<<"': "<<what;
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, ss.str());
  }
}; // struct ErrorReporter


void doubleToString(string &out, const double in)
{
  out=boost::lexical_cast<string>(in);
}

// helper grammar call to parse real functions.
template<typename Iterator>
struct FormatterGrammar: qi::grammar<Iterator, Data(), ascii::space_type>
{
  FormatterGrammar(void):
    FormatterGrammar::base_type(start_),
    errorHandle_( (ErrorReporter()) )
  {
    using qi::_1;
    using qi::_2;
    using qi::_val;
    using qi::lit;
    using qi::eps;
    using qi::lexeme;
    using ascii::char_;
    using ascii::lower;
    using ascii::alnum;
    using namespace qi::labels;
    using phoenix::at_c;
    using phoenix::push_back;

    // grammar specification in boost::spirit's EBNF-like notation
    numberString_ %= qi::double_[phoenix::bind(&doubleToString, _val, _1)]; // NOTE: it has to be string any way...
    quotedString_ %= lexeme['`' >> *(char_-'`') >> '`'];
    paramStr_     %= quotedString_ | numberString_;
    param_         = paramStr_[at_c<1>(_val)=_1,
                               at_c<0>(_val)=Data::ARGUMENT];
    value_         = (lit("value") >> '(' > ')')[at_c<0>(_val)=Data::VALUE];
    arg_           = func_ | param_;
    argVec_        = (arg_ % ',') | eps;
    funcName_     %= lexeme[lower >> *alnum];

    // NOTE: this declaration can be presented as a port of func_ rule, but it appears
    //       that due to some strange error messages it does not compile on boost::spirit
    //       1.42. possible reason of this is that %= can be called only for the strings.
    funcCall_     = ( funcName_ >> '(' >> argVec_ >> ')' )[at_c<1>(_val)=_1,
                                                           at_c<2>(_val)=_2,
                                                           at_c<0>(_val)=Data::FUNCTION];
    func_        %= ( '(' >> func_ >> ')' ) |
                    ( value_ ) |
                    ( funcCall_ );

    start_       %= func_;

    // error handling
    start_.name("main rule");
    qi::on_error<qi::fail>( start_, errorHandle_(qi::_4, qi::_3, qi::_2) );

    // this will make debug printouts fit in few lines!
    BOOST_SPIRIT_DEBUG_NODE(quotedString_);
    BOOST_SPIRIT_DEBUG_NODE(param_);
    BOOST_SPIRIT_DEBUG_NODE(value_);
    BOOST_SPIRIT_DEBUG_NODE(arg_);
    BOOST_SPIRIT_DEBUG_NODE(argVec_);
    BOOST_SPIRIT_DEBUG_NODE(funcName_);
    BOOST_SPIRIT_DEBUG_NODE(func_);
    BOOST_SPIRIT_DEBUG_NODE(start_);
  }

  qi::rule<Iterator, std::string(),     ascii::space_type> numberString_;   // parses: 123, etc...
  qi::rule<Iterator, std::string(),     ascii::space_type> quotedString_;   // parses: "abc", etc...
  qi::rule<Iterator, std::string(),     ascii::space_type> paramStr_;       // parses paramter types
  qi::rule<Iterator, Data(),            ascii::space_type> param_;          // parameter's value
  qi::rule<Iterator, Data(),            ascii::space_type> value_;          // parses special 'value()' function
  qi::rule<Iterator, Data(),            ascii::space_type> arg_;            // single function argument (return from other function or parameter)
  qi::rule<Iterator, Data::Arguments(), ascii::space_type> argVec_;         // vector of arguments (can be empty)
  qi::rule<Iterator, std::string(),     ascii::space_type> funcName_;       // name of the function
  qi::rule<Iterator, Data(),            ascii::space_type> funcCall_;       // real funciton call
  qi::rule<Iterator, Data(),            ascii::space_type> func_;           // function declaration along with brackets
  qi::rule<Iterator, Data(),            ascii::space_type> start_;          // start rule (alias to func_)
  phoenix::function<ErrorReporter>      errorHandle_;                       // generic error handle
}; // struct FormatterGrammar



inline FormatterConfig defaultConfig(void)
{
  detail::FormatterConfigData v;
  v.type_=detail::FormatterConfigData::VALUE;
  return FormatterConfig(v);
} // defaultConfig()


inline FormatterConfig parseString(const std::string &str)
{
  try
  {
    // check for special case
    if(str=="")
      return defaultConfig();

    // do the real parsing
    FormatterGrammar<string::const_iterator> parser;
    Data                                     data;
    string::const_iterator it =str.begin();
    string::const_iterator end=str.end();
    if( !phrase_parse(it, end, parser, ascii::space, data) )
      throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "phrase_parse() returned an error");
    // TODO: add (part) of what left to the error message.
    if(it!=end)
      throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "not whole string has been parsed");

    // return parsed data
    return FormatterConfig(data);
  }
  catch(const boost::exception &ex)
  {
    const string err=string("got function parser error: ") + boost::diagnostic_information(ex);
    // translate an exception to the common exception class type
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, err);
  }
} // parseString()

} // unnamed namespace


FunctionParser::FunctionParser(const std::string &formatter):
  cfg_( parseString(formatter) )
{
}

} // namespace Preprocessor
} // namespace ConfigIO
