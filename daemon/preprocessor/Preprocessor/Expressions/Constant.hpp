/*
 * Constant.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_CONSTANT_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_CONSTANT_HPP_FILE

#include "Preprocessor/Expressions/Base.hpp"

// TODO: comment this code

namespace Preprocessor
{
namespace Expressions
{

class Constant: public Base
{
public:
  explicit Constant(bool value);

  virtual bool compute(const Path &path, const Persistency::Alert &alert) const;

private:
  bool v_;
}; // class Base


struct True: public Constant
{
  explicit True(void);
}; // struct True


struct False: public Constant
{
  explicit False(void);
}; // struct False

} // namespace Expressions
} // namespace Preprocessor

#endif
