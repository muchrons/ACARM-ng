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

  Expression(Type type, const Rules &rules, const Expressions &expressions):
    type_(type),
    rules_(rules),
    expressions_(expressions)
  {
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
  Type        type_;
  Rules       rules_;
  Expressions expressions_;
}; // class Expressions

} // namespace Preprocessor
} // namespace ConfigIO

#endif
