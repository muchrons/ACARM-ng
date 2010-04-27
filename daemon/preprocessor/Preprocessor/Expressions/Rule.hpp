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
#include "Preprocessor/ExceptionInvalidPath.hpp"

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

  template<typename T>
  bool processCollection(PathCit t, const T &col) const;

  template<typename Tit>
  bool matchEach(PathCit t, const Tit begin, const Tit end) const;

  template<typename Tit>
  bool matchAny(PathCit t, const Tit begin, const Tit end) const;

  template<typename T>
  bool check(const T *e, PathCit t) const;

  bool check(const Persistency::Alert                   &e, PathCit t) const;
  bool check(const Persistency::Alert::SourceAnalyzers  &e, PathCit t) const;
  bool check(const Persistency::Analyzer                &e, PathCit t) const;
  bool check(const Persistency::Alert::ReportedHosts    &e, PathCit t) const;
  bool check(const Persistency::ReferenceURL            &e, PathCit t) const;
  bool check(const Persistency::Host                    &e, PathCit t) const;
  bool check(const Persistency::Host::ReportedServices  &e, PathCit t) const;
  bool check(const Persistency::Service                 &e, PathCit t) const;
  bool check(const Persistency::Host::ReportedProcesses &e, PathCit t) const;
  bool check(const Persistency::Process                 &e, PathCit t) const;
  bool check(const Persistency::MD5Sum                  &e, PathCit t) const;
  // final point
  bool check(const std::string                          &e, PathCit t) const;

  void throwIfEnd(PathCit t) const;
  void throwIfNotEnd(PathCit t) const;
  void throwInvalid(PathCit t) const;

  const Path                        path_;
  boost::scoped_ptr<Checkers::Mode> checker_;
}; // class Rule

} // namespace Expressions
} // namespace Preprocessor

#endif
