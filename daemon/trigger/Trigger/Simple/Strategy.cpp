/*
 * Strategy.cpp
 *
 */
#include "Trigger/Simple/Strategy.hpp"

using namespace std;

namespace Trigger
{
namespace Simple
{

// TODO

Strategy::Strategy(const std::string &name):
  Trigger::Strategy(name)
{
}

bool Strategy::matchesCriteria(const NodeType &n) const
{
  // TODO
  return false;
}

} // namespace Simple
} // namespace Filter
