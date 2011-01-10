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

bool Not::compute(const Persistency::Alert &alert) const
{
  return !e_->compute(alert);
}

} // namespace Expressions
} // namespace Preprocessor
