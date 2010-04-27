/*
 * Not.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_NOT_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_NOT_HPP_FILE

#include "Preprocessor/Expressions/Base.hpp"

// TODO: comment this code

namespace Preprocessor
{
namespace Expressions
{

class Not: public Base
{
public:
  explicit Not(BasePtrNN e);
  virtual bool compute(const Persistency::Alert &alert) const;

private:
  BasePtrNN e_;
}; // class Not

} // namespace Expressions
} // namespace Preprocessor

#endif
