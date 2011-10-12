/*
 * Not.cpp
 *
 */
#include "Preprocessor/Expressions/Not.hpp"

namespace Preprocessor
{
namespace Expressions
{

Not::Not(BasePtrNN e):
  e_(e)
{
}

bool Not::compute(const Persistency::ConstGraphNodePtrNN &node) const
{
  return !e_->compute(node);
}

} // namespace Expressions
} // namespace Preprocessor
