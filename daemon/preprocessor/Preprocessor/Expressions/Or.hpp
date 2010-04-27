/*
 * Or.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_OR_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_OR_HPP_FILE

#include <vector>

#include "Preprocessor/Expressions/Base.hpp"

// TODO: comment this code

namespace Preprocessor
{
namespace Expressions
{

class Or: public Base
{
public:
  typedef std::vector<BasePtrNN> Expressions;

  explicit Or(const Expressions e);
  virtual bool compute(const Path &path, const Persistency::Alert &alert) const;

private:
  const Expressions e_;
}; // class Or

} // namespace Expressions
} // namespace Preprocessor

#endif
