/*
 * Rule.cpp
 *
 */
#include <cassert>

#include "Preprocessor/Checkers/Equals.hpp"
#include "Preprocessor/Checkers/Contains.hpp"
#include "Preprocessor/Expressions/Rule.hpp"

namespace Preprocessor
{
namespace Expressions
{

Rule::Rule(const Path &path, Mode mode, const Value &value):
  path_(path)
{
  // create proper checker
  switch( mode.toInt() )
  {
    case Mode::EQUALS:
      checker_.reset( new Checkers::Equals(value) );
      break;

    case Mode::CONTAINS:
      checker_.reset( new Checkers::Contains(value) );
      break;

    default:
      assert(!"unknown mode requested");
      checker_.reset( new Checkers::Equals(value) );    // fallback...
      break;
  } // switch(mode)
  assert( checker_.get()!=NULL );
}

bool Rule::compute(const Persistency::Alert &alert) const
{
  assert( checker_.get()!=NULL );
  return check( alert, path_.begin() );
}

template<typename Tit>
bool Rule::matchEach(PathCit   t,     const PathCit pathEnd,
                     const Tit begin, const Tit     end) const
{
  // TODO
}

template<typename Tit>
bool Rule::matchAny(PathCit   t,     const PathCit pathEnd,
                    const Tit begin, const Tit     end) const
{
  // TODO
}

bool Rule::check(const Persistency::Alert &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Alert::SourceAnalyzers &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Analyzer &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Alert::ReportedHosts &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Host &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Host::ReportedServices &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Service &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Host::ReportedProcesses &e, PathCit t) const
{
}

bool Rule::check(const Persistency::Process &e, PathCit t) const
{
}

bool Rule::check(const std::string &e, PathCit t) const
{
}

} // namespace Expressions
} // namespace Preprocessor
