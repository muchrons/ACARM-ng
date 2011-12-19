/*
 * FormatterBuilder.cpp
 *
 */
#include "Preprocessor/Expressions/FormatterBuilder.hpp"
#include "Preprocessor/Formatters/Abs.hpp"
#include "Preprocessor/Formatters/Add.hpp"
#include "Preprocessor/Formatters/Ceil.hpp"
#include "Preprocessor/Formatters/Constant.hpp"
#include "Preprocessor/Formatters/Div.hpp"
#include "Preprocessor/Formatters/Floor.hpp"
#include "Preprocessor/Formatters/Max.hpp"
#include "Preprocessor/Formatters/Min.hpp"
#include "Preprocessor/Formatters/Mul.hpp"
#include "Preprocessor/Formatters/Round.hpp"
#include "Preprocessor/Formatters/Sig.hpp"
#include "Preprocessor/Formatters/Sub.hpp"
#include "Preprocessor/Formatters/TimestampProc.hpp"

using namespace Preprocessor::Formatters;

typedef ConfigIO::Preprocessor::FormatterConfig::Wrapper Wrapper;


namespace Preprocessor
{
namespace Expressions
{

FormatterBuilder::ExceptionInvalidNumberOfArguments::ExceptionInvalidNumberOfArguments(const Location    &where,
                                                                                       const std::string &func,
                                                                                       const char        *details):
  Exception(where, cc("invalid number of arguments for formatter function '", func, "': ", details) )
{
}


FormatterBuilder::ExceptionUnknownFunction::ExceptionUnknownFunction(const Location &where, const std::string &func):
  Exception(where, cc("invalid function '", func, "'") )
{
}


FormatterBuilder::FormatterBuilder(Formatters::ValuePtrNN value):
  value_(value)
{
}


Formatters::BasePtrNN FormatterBuilder::build(const ConfigIO::Preprocessor::FormatterConfig &cfg) const
{
  return buildImpl( cfg.get() );
}


Formatters::BasePtrNN FormatterBuilder::buildImpl(const ConfigIO::Preprocessor::FormatterConfig::Wrapper cfg) const
{
  if( cfg.isArgument() )
    return BasePtrNN( new Constant(cfg.argument()) );

  if( cfg.isValue() )
    return value_;

  if( cfg.isFunction() )
    return buildFunction(cfg);

  // just in case...
  assert(!"unknown element - please update code");
  throw std::logic_error("unknown element - please update code");
  return value_;    // this code is never reached
}


Formatters::BasePtrNN FormatterBuilder::buildFunction(const ConfigIO::Preprocessor::FormatterConfig::Wrapper cfg) const
{
  assert( cfg.isFunction() );
  // prepare arguments
  Preprocessor::Formatters::Base::Formatters args;
  args.reserve( cfg.argCount() );
  for(size_t i=0; i<cfg.argCount(); ++i)
    args.push_back( buildImpl( cfg.param(i) ) );

  //
  // use them to create final function
  //

  // abs()
  if( cfg.name()=="abs" )
  {
    if( args.size()!=1 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 1 argument");
    return BasePtrNN( new Abs(args[0]) );
  }

  // add()
  if( cfg.name()=="add" )
  {
    if( args.size()<2 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects at least 2 arguments");
    return BasePtrNN( new Add(args) );
  }

  // ceil()
  if( cfg.name()=="ceil" )
  {
    if( args.size()!=1 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 1 argument");
    return BasePtrNN( new Ceil(args[0]) );
  }

  // div()
  if( cfg.name()=="div" )
  {
    if( args.size()!=2 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 2 arguments");
    return BasePtrNN( new Div(args[0], args[1]) );
  }

  // floor()
  if( cfg.name()=="floor" )
  {
    if( args.size()!=1 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 1 argument");
    return BasePtrNN( new Floor(args[0]) );
  }

  // max()
  if( cfg.name()=="max" )
  {
    if( args.size()<2 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects at least 2 arguments");
    return BasePtrNN( new Max(args) );
  }

  // min()
  if( cfg.name()=="min" )
  {
    if( args.size()<2 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects at least 2 arguments");
    return BasePtrNN( new Min(args) );
  }

  // mul()
  if( cfg.name()=="mul" )
  {
    if( args.size()<2 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects at least 2 arguments");
    return BasePtrNN( new Mul(args) );
  }

  // round()
  if( cfg.name()=="round" )
  {
    if( args.size()!=1 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 1 argument");
    return BasePtrNN( new Round(args[0]) );
  }

  // sig()
  if( cfg.name()=="sig" )
  {
    if( args.size()!=1 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 1 argument");
    return BasePtrNN( new Sig(args[0]) );
  }

  // sub()
  if( cfg.name()=="sub" )
  {
    if( args.size()!=2 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 2 arguments");
    return BasePtrNN( new Sub(args[0], args[1]) );
  }

  // tsProc()
  if( cfg.name()=="tsProc" )
  {
    if( args.size()!=2 )
      throw ExceptionInvalidNumberOfArguments(SYSTEM_SAVE_LOCATION, cfg.name(), "function expects exactly 2 arguments");
    return BasePtrNN( new TimestampProc(args[0], cfg.param(1).argument()) );
  }

  // oops...
  throw ExceptionUnknownFunction(SYSTEM_SAVE_LOCATION, cfg.name());
}

} // namespace Expressions
} // namespace Preprocessor
