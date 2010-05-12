/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Trigger/GG/Strategy.hpp"

using namespace std;

namespace Trigger
{
namespace GG
{

Strategy::Strategy(void):
  Trigger::Strategy("gg")
{
}

bool Strategy::matchesCriteria(const NodeType &n) const
{
  // TODO
  return false;
}

void Strategy::trigger(const NodeType &n)
{
  // TODO
}

} // namespace GG
} // namespace Trigger
