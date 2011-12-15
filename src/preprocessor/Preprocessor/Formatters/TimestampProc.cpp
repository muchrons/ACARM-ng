/*
 * TimestampProc.cpp
 *
 */
#include <sstream>
#include <cmath>
#include <cassert>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/exception/all.hpp>

#include "Preprocessor/Formatters/TimestampProc.hpp"
#include "Commons/NumberWrapper.hpp"
#include "Commons/Convert.hpp"

using namespace std;
namespace qi     =boost::spirit::qi;
namespace ascii  =boost::spirit::ascii;
namespace phoenix=boost::phoenix;


namespace Preprocessor
{
namespace Formatters
{

TimestampProc::ExceptionInvalidFormat::ExceptionInvalidFormat(const Location &where, const std::string &what):
  Exception(where, cc("invalid format: ", what) )
{
}

TimestampProc::ExceptionConvertionError::ExceptionConvertionError(const Location &where, const time_t ts):
  Exception(where, cc("gmtime_r() failed to convert timestamp ", ts) )
{
}


TimestampProc::TimestampProc(BasePtrNN ts, const std::string &format):
  Base("tsproc", Formatters(1, ts) ),
  format_(format)
{
  LOGMSG_INFO_S(log_)<<"setting format string to: '"<<format_<<"'";
  formatTimestamp(0);   // try using this format for sample convertion. this will cause
                        // exception in case format is invalid.
}


std::string TimestampProc::execImpl(const Arguments &args) const
{
  assert(args.size()==1);
  const Commons::NumberWrapper num(args[0]);
  if( !num.isNumber() )
    throw ExceptionNotANumber(SYSTEM_SAVE_LOCATION, args[0]);
  // process
  const double fp=nearbyint( num.get() );
  const time_t ts=boost::numeric_cast<time_t>(fp);
  return formatTimestamp(ts);
}


namespace
{

template<typename Iterator>
struct FormatterGrammar: qi::grammar<Iterator, stringstream()/*, ascii::space_type*/>
{
  explicit FormatterGrammar(const time_t ts):
    FormatterGrammar::base_type(start_)
  {
    // convert timestamp to broken-down field representation
    if( gmtime_r(&ts, &bts_)!=&bts_ )
      throw TimestampProc::ExceptionConvertionError(SYSTEM_SAVE_LOCATION, ts);

    using qi::_1;
    //using qi::_2;
    using qi::_val;
    using qi::lit;
    using qi::eps;
    using qi::lexeme;
    using qi::fail;
    using qi::on_error;
    using phoenix::val;
    using phoenix::construct;
    using ascii::char_;
    //using ascii::lower;
    //using ascii::alnum;
    using namespace qi::labels;
    //using phoenix::at_c;
    //using phoenix::push_back;
    //using boost::fusion::at_c;

    /*
    // grammar specification in boost::spirit's EBNF-like notation
    quotedString_%= lexeme['"' >> *(char_-'"') >> '"'];
    param_        = quotedString_[at_c<1>(_val)=_1,
                  at_c<0>(_val)=Data::ARGUMENT];
    value_        = (lit("value") >> '(' > ')')[at_c<0>(_val)=Data::VALUE];
    arg_          = func_ | param_;
    argVec_       = (arg_ % ',') | eps;
    funcName_    %= lexeme[lower >> *alnum];
    func_        %= ( '(' >> func_ >> ')' ) |
      ( value_ ) |
      ( funcName_ >> '(' >> argVec_ >> ')' )[at_c<1>(_val)=_1,
      at_c<2>(_val)=_2,
      at_c<0>(_val)=Data::FUNCTION];
    start_       %= func_;
    */
    // special fields
    //dtField_ = (char_('Y') [_val=1900+bts_.tm_year]) |
    //           (char_('m') [_val=1+bts_.tm_mon])
               ;
    // main part
    exprImpl_ = (lit("%%") [_r1<<"%"])           |
              //lexeme['%' > dtField_] [_val<<_1] |
              (char_ [_r1<<_1])
              ;

    expr_  = ( exprImpl_(_r1) >> expr_(_r1) ) | eps;
    start_ = expr_(_val);

    // TODO: work on this error handling...
    on_error<fail>
      (
       start_,
       std::cout
       << val("Error! Expecting ")
       << _4                               // what failed?
       << val(" here: \"")
       << construct<std::string>(_3, _2)   // iterators to error-pos, end
       << val("\"")
       << std::endl
      );

  }

private:
  struct tm                                             bts_;       // field-broken timestamp
  /*
  qi::rule<Iterator, std::string(),     ascii::space_type> quotedString_;   // parses: "abc", etc...
  qi::rule<Iterator, Data(),            ascii::space_type> param_;          // parameter's value
  qi::rule<Iterator, Data(),            ascii::space_type> value_;          // parses special 'value()' function
  qi::rule<Iterator, Data(),            ascii::space_type> arg_;            // single function argument (return from other function or parameter)
  qi::rule<Iterator, Data::Arguments(), ascii::space_type> argVec_;         // vector of arguments (can be empty)
  qi::rule<Iterator, std::string(),     ascii::space_type> funcName_;       // name of the function
  qi::rule<Iterator, Data(),            ascii::space_type> func_;           // function declaration along with brackets
  qi::rule<Iterator, Data(),            ascii::space_type> start_;          // start rule (alias to func_)
  */
  qi::rule<Iterator, void(stringstream&)/*, ascii::space_type*/> dtField_;  // date/time single field's value
  qi::rule<Iterator, void(stringstream&)/*, ascii::space_type*/> exprImpl_; // actuall expression's implementation
  qi::rule<Iterator, void(stringstream&)/*, ascii::space_type*/> expr_;     // expression itself
  qi::rule<Iterator, stringstream()/*, ascii::space_type*/>      start_;    // start rule
}; // struct FormatterGrammar
} // unnamed namespace


std::string TimestampProc::formatTimestamp(const time_t ts) const
{
  try
  {
    FormatterGrammar<string::const_iterator> parser(ts);
    stringstream                             ss;
    string::const_iterator it =format_.begin();
    string::const_iterator end=format_.end();
    if( !phrase_parse(it, end, parser, ascii::space, ss) )
      throw ExceptionInvalidFormat(SYSTEM_SAVE_LOCATION, "phrase_parse() returned an error");
    // TODO: limit left string to the some reasonable length
    if(it!=end)
      throw ExceptionInvalidFormat(SYSTEM_SAVE_LOCATION, "not whole string has been parsed: '" + string(it, end) + "'");

    // return parsed data
    return ss.str();
  }
  catch(const boost::exception &ex)
  {
    const string err=string("format parse error: ") + boost::diagnostic_information(ex);
    // translate an exception to the common exception class type
    throw ExceptionInvalidFormat(SYSTEM_SAVE_LOCATION, err);
  }
  assert(!"this code is never reached");
}

} // namespace Formatters
} // namespace Preprocessor
