/*
 * CommonTestBase.t.hpp
 *
 */
#ifndef INCLUDE_MANYTOMANY_COMMONTESTBASE_HPP_FILE
#define INCLUDE_MANYTOMANY_COMMONTESTBASE_HPP_FILE

#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

namespace Algo
{

struct CommonTestBase: private TestHelpers::Persistency::TestStubs
{
  Persistency::HostPtrNN makeHost(const char *host) const;

  Persistency::AlertPtrNN makeAlert(const char *hostSrc1,
                                    const char *hostSrc2,
                                    const char *hostDst1,
                                    const char *hostDst2) const;

  Persistency::GraphNodePtrNN makeLeaf(Persistency::AlertPtrNN alert) const;
}; // struct CommonTestBase

} // namespace Algo

#endif
