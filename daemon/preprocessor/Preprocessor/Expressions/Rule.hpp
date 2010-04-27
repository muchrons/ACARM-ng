/*
 * Rule.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE

#include <boost/scoped_ptr.hpp>

#include "ConfigIO/Preprocessor/Rule.hpp"
#include "Preprocessor/Checkers/Mode.hpp"
#include "Preprocessor/Expressions/Base.hpp"

// TODO: comment this code

namespace Preprocessor
{
namespace Expressions
{

class Rule: public Base
{
public:
  typedef ConfigIO::Preprocessor::Rule::Mode  Mode;
  typedef ConfigIO::Preprocessor::Rule::Value Value;

  Rule(const Path &path, Mode mode, const Value &value);
  virtual bool compute(const Persistency::Alert &alert) const;

private:
  typedef Path::const_iterator PathCit;

  template<typename Tit>
  bool matchEach(PathCit   t,     const PathCit pathEnd,
                 const Tit begin, const Tit     end) const;

  template<typename Tit>
  bool matchAny(PathCit   t,     const PathCit pathEnd,
                const Tit begin, const Tit     end) const;

  bool check(const Persistency::Alert                   &e, PathCit t) const;
  bool check(const Persistency::Alert::SourceAnalyzers  &e, PathCit t) const;
  bool check(const Persistency::Analyzer                &e, PathCit t) const;
  bool check(const Persistency::Alert::ReportedHosts    &e, PathCit t) const;
  bool check(const Persistency::Host                    &e, PathCit t) const;
  bool check(const Persistency::Host::ReportedServices  &e, PathCit t) const;
  bool check(const Persistency::Service                 &e, PathCit t) const;
  bool check(const Persistency::Host::ReportedProcesses &e, PathCit t) const;
  bool check(const Persistency::Process                 &e, PathCit t) const;
  // final point
  bool check(const std::string                          &e, PathCit t) const;

  const Path                        path_;
  boost::scoped_ptr<Checkers::Mode> checker_;
}; // class Rule

} // namespace Expressions
} // namespace Preprocessor

#endif
