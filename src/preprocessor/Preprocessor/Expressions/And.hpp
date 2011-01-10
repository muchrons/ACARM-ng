/*
 * And.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_AND_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_AND_HPP_FILE

#include "Preprocessor/Expressions/Base.hpp"
#include "Preprocessor/Expressions/Set.hpp"

namespace Preprocessor
{
namespace Expressions
{

/** \brief expression representing AND statement.
 */
class And: public Base
{
public:
  /** \brief expressions to be ANDed. */
  typedef Set Expressions;

  /** \brief create instance.
   *  \param e expressions to be ANDed.
   */
  explicit And(const Expressions e);

  /** \brief computes answer for given alert.
   *  \param alert alert to be chekced/computed.
   *  \return true if alert matches given criteria, false otherwise.
   */
  virtual bool compute(const Persistency::Alert &alert) const;

private:
  const Expressions e_;
}; // class And

} // namespace Expressions
} // namespace Preprocessor

#endif
