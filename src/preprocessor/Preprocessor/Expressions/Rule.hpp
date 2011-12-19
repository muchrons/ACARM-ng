/*
 * Rule.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_RULE_HPP_FILE

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "ConfigIO/Preprocessor/Rule.hpp"
#include "DataFacades/StrAccess/StrAccess.hpp"
#include "Preprocessor/Checkers/Mode.hpp"
#include "Preprocessor/Formatters/Base.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Expressions/Base.hpp"
#include "Preprocessor/ExceptionInvalidPath.hpp"


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
  typedef ConfigIO::Preprocessor::Rule::Mode  Mode;
  /** \brief forward definition of rule's value to operate on. */
  typedef ConfigIO::Preprocessor::Rule::Value Value;
  /** \brief short name for Path. */
  typedef DataFacades::StrAccess::Path        Path;

  /** \brief create complex rule.
   *  \param path  path to alert's element
   *  \param mode  mode of checking.
   *  \param value value to operate on.
   *  \param fmt   formatter config to apply.
   */
  Rule(const Path &path, Mode mode, const Value &value, const ConfigIO::Preprocessor::FormatterConfig &fmt);

  /** \brief computes answer for given alert.
   *  \param node (meta-)alert to be chekced/computed.
   *  \return true if alert matches given criteria, false otherwise.
   */
  virtual bool compute(const Persistency::ConstGraphNodePtrNN &node) const;

private:
  const Path                        path_;
  boost::scoped_ptr<Checkers::Mode> checker_;
  Formatters::ValuePtrNN            formatterValue_;
  Formatters::BasePtrNN             baseFormatter_;
}; // class Rule

} // namespace Expressions
} // namespace Preprocessor

#endif
