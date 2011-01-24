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
  }
  Type;
};
} // namespace detail


/** \brief class representing expression.
 */
class Expression
{
public:
  /** \brief type of expression to be used. */
  typedef System::Enum<detail::ExpressionType> Type;
  /** \brief type holding set of rules. */
  typedef std::vector<Rule>                    Rules;
  /** \brief type holding set of expressions. */
  typedef std::vector<Expression>              Expressions;

  /** \brief named c-tor creating AND expression.
   *  \param expressions expressions to take part in computation.
   *  \return created object.
   */
  static Expression makeAnd(const Expressions &expressions)
  {
    return Expression(Type::AND, Rules(), expressions);
  }
  /** \brief named c-tor creating OR expression.
   *  \param expressions expressions to take part in computation.
   *  \return created object.
   */
  static Expression makeOr(const Expressions &expressions)
  {
    return Expression(Type::OR, Rules(), expressions);
  }
  /** \brief named c-tor creating NOT expression.
   *  \param expression expression to be negated.
   *  \return created object.
   */
  static Expression makeNot(const Expression &expression)
  {
    Expressions expressions;
    expressions.push_back(expression);
    return Expression(Type::NOT, Rules(), expressions);
  }
  /** \brief named c-tor creating terminal expression.
   *  \param rule rule to create expression from.
   *  \return created object.
   */
  static Expression makeTerm(const Rule &rule)
  {
    Rules rules;
    rules.push_back(rule);
    return Expression(Type::TERM, rules, Expressions() );
  }

  /** \brief gets expression type.
   *  \return expression type.
   */
  Type getType(void) const
  {
    return type_;
  }
  /** \brief gets rules held inside.
   *  \return rules hold inside.
   */
  const Rules &getRules(void) const
  {
    return rules_;
  }
  /** \brief gets expressions held inside.
   *  \return expressions hold inside.
   */
  const Expressions &getExpressions(void) const
  {
    return expressions_;
  }

private:
  // named c-tors are to be used instead of direct object creation.
  Expression(Type type, const Rules &rules, const Expressions &expressions):
    type_(type),
    rules_(rules),
    expressions_(expressions)
  {
    assert( getRules().size()==0 || getExpressions().size()==0 );
  }

  Type        type_;
  Rules       rules_;
  Expressions expressions_;
}; // class Expressions

} // namespace Preprocessor
} // namespace ConfigIO

#endif
