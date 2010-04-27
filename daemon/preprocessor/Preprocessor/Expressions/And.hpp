/*
 * And.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_AND_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_AND_HPP_FILE

#include <vector>

#include "Preprocessor/Expressions/Base.hpp"

// TODO: comment this code

namespace Preprocessor
{
namespace Expressions
{

class And: public Base
{
public:
  typedef std::vector<BasePtrNN> Expressions;

  explicit And(const Expressions e);
  virtual bool compute(const Persistency::Alert &alert) const;

private:
  const Expressions e_;
}; // class And

} // namespace Expressions
} // namespace Preprocessor

#endif
