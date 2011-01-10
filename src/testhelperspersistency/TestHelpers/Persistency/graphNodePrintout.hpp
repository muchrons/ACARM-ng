/*
 * graphNodePrinteout.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_GRAPHNODEPRINTOUT_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_GRAPHNODEPRINTOUT_HPP_FILE

/* public header */

#include <vector>

#include "Persistency/GraphNodePtr.hpp"


namespace TestHelpers
{
namespace Persistency
{

void graphNodePrintout(const std::vector< ::Persistency::GraphNodePtr > &v);
void graphNodePrintout(const std::vector< ::Persistency::GraphNodePtrNN > &v);
void graphNodePrintout(::Persistency::GraphNodePtr n);
void graphNodePrintout(::Persistency::GraphNodePtrNN n);

} // namespace Persistency
} // namespace TestHelpers

#endif
