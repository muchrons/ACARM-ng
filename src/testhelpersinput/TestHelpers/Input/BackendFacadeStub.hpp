/*
 * BackendFacadeStub.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_INPUT_BACKENDFACADESTUB_HPP_FILE
#define INCLUDE_TESTHELPERS_INPUT_BACKENDFACADESTUB_HPP_FILE

/* public header */

#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Input/BackendFacade.hpp"

namespace TestHelpers
{
namespace Input
{
namespace detail
{
struct BackendFacadeStubData
{
  DataFacades::AnalyzersCreator ac_;
}; // struct BackendFacadeStubData
} // namespace detail

class BackendFacadeStub: private detail::BackendFacadeStubData,
                         public  ::Input::BackendFacade
{
public:
  explicit BackendFacadeStub(::Persistency::IO::ConnectionPtrNN conn);
}; // class BackendFacadeStub

} // namespace Input
} // namespace TestHelpers

#endif
