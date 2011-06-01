/*
 * Similarity.cpp
 *
 */
#include "Algo/Diff/Similarity.hpp"

namespace Algo
{
namespace Diff
{

Similarity::ExceptionInvalidValue::ExceptionInvalidValue(const Location &where, const char *msg, const double value):
  Exception(where, cc("invalid simialarity ", value, ": ", msg) )
{
}

} // namespace Diff
} // namespace Algo
