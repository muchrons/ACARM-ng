/*
 * Or.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_OR_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_OR_HPP_FILE

#include "Preprocessor/Expressions/Base.hpp"
#include "Preprocessor/Expressions/Set.hpp"

namespace Preprocessor
{
namespace Expressions
{

/** \brief logical OR expression.
 */
class Or: public Base
{
public:
  /** \brief expressions to be ORed. */
  typedef Set Expressions;

  /** \brief create expression.
   *  \e expressions to be ORed.
   */
  explicit Or(const Expressions e);

  /** \brief computes answer for given alert.
   *  \param node (meta-)alert to be chekced/computed.
   *  \return true if alert matches given criteria, false otherwise.
   */
  virtual bool compute(const Persistency::ConstGraphNodePtrNN &node) const;

private:
  const Expressions e_;
}; // class Or

} // namespace Expressions
} // namespace Preprocessor

#endif
