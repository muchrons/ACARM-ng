/*
 * Constant.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_CONSTANT_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_CONSTANT_HPP_FILE

#include "Preprocessor/Expressions/Base.hpp"

namespace Preprocessor
{
namespace Expressions
{

/** \brief constant expression.
 */
class Constant: public Base
{
public:
  /** \brief create constant expression.
   *  \param value constant value to be always returned.
   */
  explicit Constant(bool value);

  /** \brief computes answer for given alert.
   *  \param node (meta-)alert to be chekced/computed.
   *  \return true if alert matches given criteria, false otherwise.
   */
  virtual bool compute(const Persistency::ConstGraphNodePtrNN &node) const;

private:
  bool v_;
}; // class Base


/** \brief true condition.
 */
struct True: public Constant
{
  /** \brief create object.
   */
  True(void);
}; // struct True


/** \brief false condition.
 */
struct False: public Constant
{
  /** \brief create object.
   */
  False(void);
}; // struct False

} // namespace Expressions
} // namespace Preprocessor

#endif
