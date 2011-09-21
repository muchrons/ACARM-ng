/*
 * Constant.cpp
 *
 */
#include "Preprocessor/Expressions/Constant.hpp"

namespace Preprocessor
{
namespace Expressions
{

Constant::Constant(bool value):
  v_(value)
{
}

bool Constant::compute(const Persistency::ConstGraphNodePtrNN &/*node*/) const
{
  return v_;
}


True::True(void):
  Constant(true)
{
}


False::False(void):
  Constant(false)
{
}

} // namespace Expressions
} // namespace Preprocessor
