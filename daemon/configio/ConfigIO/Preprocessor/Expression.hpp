/*
 * Expression.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_EXPRESSION_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_EXPRESSION_HPP_FILE

/* public header */

#include <vector>

#include "System/Enum.hpp"
#include "ConfigIO/Preprocessor/Rule.hpp"

// TODO: comment this code.

namespace ConfigIO
{
namespace Preprocessor
{
namespace detail
{
/** \brief implementation helper.
 */
struct ExpressionType
{
  /** \brief implementaiton enum. only to be used via Expression::Type. */
  typedef enum
  {
    AND,
    OR,
    NOT,
    TERM
  } Type;
};
} // namespace detail


/** \brief class representing expression.
 */
class Expression
{
public:
  /** \brief type of expression to be used. */
  typedef System::Enum<detail::ExpressionType> Type;
  typedef std::vector<Rule>                    Rules;
  typedef std::vector<Expression>              Expressions;

  static makeAnd(const Rules &rules, const Expressions &expressions)
  {
    return Expression(Type::AND, rules, expressions);
  }

  static makeOr(const Rules &rules, const Expressions &expressions)
  {
    return Expression(Type::OR, rules, expressions);
  }

  static makeNot(const Rule &rule)
  {
    Rules rules;
    rules.push_back(rule);
    return Expression(Type::NOT, rules, Expressions() );
  }

  static makeNot(const Expression &expression)
  {
    Expressions expressions;
    expressions.push_back(expression);
    return Expression(Type::NOT, Rules(), expression);
  }

  static makeTerm(const Rule &rule)
  {
    Rules rules;
    rules.push_back(rule);
    return Expression(Type::TERM, rules, Expressions() );
  }

  Type getType(void) const
  {
    return type_;
  }
  const Rules &getRules(void) const
  {
    return rules_;
  }
  const Expressions &getExpressions(void) const
  {
    return expressions_;
  }

private:
  Expression(Type type, const Rules &rules, const Expressions &expressions):
    type_(type),
    rules_(rules),
    expressions_(expressions)
  {
  }

  Type        type_;
  Rules       rules_;
  Expressions expressions_;
}; // class Expressions

} // namespace Preprocessor
} // namespace ConfigIO

#endif
