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

struct ErrorReporter
{
  template <typename, typename, typename>
  struct result { typedef void type; };

  explicit ErrorReporter(const char *details):
    details_(details)
  {
  }

  template <typename Iterator>
  void operator()(qi::info const &what, Iterator errPos, Iterator last) const
  {
    stringstream ss;
    ss<<"parse error near '"<<string(errPos, last)<<"' - "<<details_<<": "<<what;
    throw TimestampProc::ExceptionInvalidFormat(SYSTEM_SAVE_LOCATION, ss.str());
  }

private:
  const char *details_;
}; // struct ErrorReporter


template<typename Iterator>
struct FormatterGrammar: qi::grammar<Iterator, stringstream()>
{
  explicit FormatterGrammar(const time_t ts):
    FormatterGrammar::base_type(start_),
    errorHandleExpr_( (ErrorReporter("format does not match grammar")) ),
    errorHandleDTF_( (ErrorReporter("invalid modifier provided")) )
  {
    // convert timestamp to broken-down field representation
    if( gmtime_r(&ts, &bts_)!=&bts_ )
      throw TimestampProc::ExceptionConvertionError(SYSTEM_SAVE_LOCATION, ts);

    using qi::_val;
    using qi::lit;
    using qi::eps;
    using qi::lexeme;
    using qi::fail;
    using qi::on_error;
    using phoenix::val;
    using phoenix::construct;
    using ascii::char_;
    using namespace qi::labels;

    // special fields
    dtField_ = (char_('%') [_r1<<'%'])                                                  |   // '%' sign
               (char_('Y') [_r1<<setfill('0'), _r1<<setw(4), _r1<<(1900+bts_.tm_year)]) |   // 4-digit year
               (char_('m') [_r1<<setfill('0'), _r1<<setw(2), _r1<<(1+bts_.tm_mon)])     |   // 2-digit month
               (char_('d') [_r1<<setfill('0'), _r1<<setw(2), _r1<<(bts_.tm_mday)])      |   // 2-digit day
               (char_('H') [_r1<<setfill('0'), _r1<<setw(2), _r1<<(bts_.tm_hour)])      |   // 2-digit hour
               (char_('M') [_r1<<setfill('0'), _r1<<setw(2), _r1<<(bts_.tm_min)])       |   // 2-digit minute
               (char_('S') [_r1<<setfill('0'), _r1<<setw(2), _r1<<(bts_.tm_sec)])           // 2-digit second
               ;
    dtField_.name("date-time field");
    // main part
    exprImpl_ = lexeme['%' > dtField_(_r1)] |
                (char_ [_r1<<_1])
                ;
    exprImpl_.name("expression detail");
    // accept any sequence of strings/format firelds
    expr_  = ( exprImpl_(_r1) >> expr_(_r1) ) | eps;
    expr_.name("expressions sequence");
    // just use single stringstream for gathering the result
    start_ = expr_(_val);
    start_.name("start rule");

    // prepare error handling
    on_error<fail>( dtField_, errorHandleDTF_ (qi::_4, qi::_3, qi::_2) );
    on_error<fail>( start_,   errorHandleExpr_(qi::_4, qi::_3, qi::_2) );
  }

private:
  struct tm                               bts_;             // field-broken timestamp
  qi::rule<Iterator, void(stringstream&)> dtField_;         // date/time single field's value
  qi::rule<Iterator, void(stringstream&)> exprImpl_;        // actuall expression's implementation
  qi::rule<Iterator, void(stringstream&)> expr_;            // expression itself
  qi::rule<Iterator, stringstream()>      start_;           // start rule
  phoenix::function<ErrorReporter>        errorHandleExpr_; // reaction on error - main
  phoenix::function<ErrorReporter>        errorHandleDTF_;  // reaction on error - format field
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
