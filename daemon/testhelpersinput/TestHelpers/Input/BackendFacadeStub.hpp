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
  BackendFacadeStubData(::Persistency::IO::ConnectionPtrNN  conn,
                        ::Persistency::IO::Transaction     &t);

  ::Input::detail::AnalyzersMap am_;
  ::Input::CommonDataPtrNN      cd_;
}; // struct BackendFacadeStubData
} // namespace detail

class BackendFacadeStub: private detail::BackendFacadeStubData,
                         public  ::Input::BackendFacade
{
public:
  BackendFacadeStub(::Persistency::IO::ConnectionPtrNN  conn,
                    ::Persistency::IO::Transaction     &t);
}; // class BackendFacadeStub

} // namespace Input
} // namespace TestHelpers

#endif
