/*
 * Not.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_NOT_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_NOT_HPP_FILE

#include "Preprocessor/Expressions/Base.hpp"

namespace Preprocessor
{
namespace Expressions
{

/** \brief expression negating given condition.
 */
class Not: public Base
{
public:
  /** \brief create instance.
   *  \param e expression ot be negated.
   */
  explicit Not(BasePtrNN e);

  /** \brief computes answer for given alert.
   *  \param node (meta-)alert to be chekced/computed.
   *  \return true if alert matches given criteria, false otherwise.
   */
  virtual bool compute(const Persistency::ConstGraphNodePtrNN &node) const;

private:
  BasePtrNN e_;
}; // class Not

} // namespace Expressions
} // namespace Preprocessor

#endif
