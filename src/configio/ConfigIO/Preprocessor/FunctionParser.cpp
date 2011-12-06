/*
 * FunctionParser.cpp
 *
 */
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include "ConfigIO/Preprocessor/FunctionParser.hpp"

namespace qi     =boost::spirit::qi;
namespace ascii  =boost::spirit::ascii;
namespace phoenix=boost::phoenix;

using namespace std;


typedef ConfigIO::Preprocessor::detail::FormatterConfigData Data;


// tell boost::fusion about data structure to be used
BOOST_FUSION_ADAPT_STRUCT(
      Data,
      (Data::Type,      type_)
      (std::string,     str_ )
      (Data::Arguments, args_)
    )

namespace ConfigIO
{
namespace Preprocessor
{

namespace
{

template<typename Iterator>
struct FormatterGrammar: qi::grammar<Iterator, Data(), ascii::space_type>
{
  FormatterGrammar(void):
    FormatterGrammar::base_type(start_)
  {
    using qi::_1;
    using qi::_val;
    //using qi::lit;
    //using qi::lexeme;
    using ascii::char_;
    //using ascii::string;
    using namespace qi::labels;

    using phoenix::at_c;
    //using phoenix::push_back;

    quotedString_ = '"' >> (*(char_-'"'))/*[_val=_1]*/ >> '"';
    paramStr_     = quotedString_[at_c<1>(_val)=_1 , at_c<0>(_val)=Data::ARGUMENT];
    start_       %= paramStr_;
  }

  qi::rule<Iterator, std::string(), ascii::space_type> quotedString_;
  qi::rule<Iterator, Data(),        ascii::space_type> paramStr_;
  qi::rule<Iterator, Data(),        ascii::space_type> start_;

  /*
  qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
  qi::rule<Iterator, std::string(), ascii::space_type> text;
  qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
  qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
  */
}; // struct FormatterGrammar



inline FormatterConfig defaultConfig(void)
{
  detail::FormatterConfigData v;
  v.type_=detail::FormatterConfigData::VALUE;
  return FormatterConfig(v);
} // defaultConfig()


inline FormatterConfig parseString(const std::string &str)
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
  if(it!=end)
    throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "not whole string has been parsed");

  // return parsed data
  return FormatterConfig(data);
} // parseString()

} // unnamed namespace


FunctionParser::FunctionParser(const std::string &formatter):
  cfg_( parseString(formatter) )
{
}

} // namespace Preprocessor
} // namespace ConfigIO
