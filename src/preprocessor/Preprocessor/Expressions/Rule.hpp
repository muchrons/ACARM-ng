/*
 * Rule.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "ConfigIO/Preprocessor/Rule.hpp"
#include "Persistency/Facades/StrAccess/StrAccess.hpp"
#include "Preprocessor/Checkers/Mode.hpp"
#include "Preprocessor/Expressions/Base.hpp"
#include "Preprocessor/ExceptionInvalidPath.hpp"

// TODO: to be reimplemented with Persistency::Facades::StrAccess

namespace Preprocessor
{
namespace Expressions
{

/** \brief expression representing complex rule.
 */
class Rule: public Base
{
public:
  /** \brief forward definition of rule's mode. */
  typedef ConfigIO::Preprocessor::Rule::Mode    Mode;
  /** \brief forward definition of rule's value to operate on. */
  typedef ConfigIO::Preprocessor::Rule::Value   Value;
  /** \brief short name for Path. */
  typedef Persistency::Facades::StrAccess::Path Path;

  /** \brief create complex rule.
   *  \param path  path to alert's element
   *  \param mode  mode of checking.
   *  \param value value to operate on.
   */
  Rule(const Path &path, Mode mode, const Value &value);

  /** \brief computes answer for given alert.
   *  \param alert alert to be chekced/computed.
   *  \return true if alert matches given criteria, false otherwise.
   */
  virtual bool compute(const Persistency::Alert &alert) const;

private:
#if 0
  typedef Path::const_iterator PathCit;

  template<typename T>
  bool processCollection(PathCit t, const T &col) const;

  template<typename Tit>
  bool matchEach(PathCit t, const Tit begin, const Tit end) const;

  template<typename Tit>
  bool matchAny(PathCit t, const Tit begin, const Tit end) const;

  template<typename T>
  bool check(const T *e, PathCit t) const;
  template<typename T>
  bool check(const boost::shared_ptr<T> e, PathCit t) const;

  bool check(const Persistency::Alert            &e, PathCit t) const;
  bool check(const Persistency::Alert::Analyzers &e, PathCit t) const;
  bool check(const Persistency::Analyzer         &e, PathCit t) const;
  bool check(const Persistency::Alert::Hosts     &e, PathCit t) const;
  bool check(const Persistency::ReferenceURL     &e, PathCit t) const;
  bool check(const Persistency::Host             &e, PathCit t) const;
  bool check(const Persistency::Host::Services   &e, PathCit t) const;
  bool check(const Persistency::Service          &e, PathCit t) const;
  bool check(const Persistency::Host::Processes  &e, PathCit t) const;
  bool check(const Persistency::Process          &e, PathCit t) const;
  bool check(const Persistency::MD5Sum           &e, PathCit t) const;
  // final point
  bool check(const std::string                   &e, PathCit t) const;

  void throwIfEnd(const Exception::Location &where, PathCit t) const;
  void throwIfNotEnd(const Exception::Location &where, PathCit t) const;
  void throwInvalid(const Exception::Location &where, PathCit t) const;
#endif

  const Path                        path_;
  boost::scoped_ptr<Checkers::Mode> checker_;
}; // class Rule

} // namespace Expressions
} // namespace Preprocessor

#endif
