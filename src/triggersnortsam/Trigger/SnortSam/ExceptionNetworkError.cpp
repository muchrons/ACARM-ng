/*
 * ExceptionNetworkError.cpp
 *
 */
#include "Trigger/SnortSam/ExceptionNetworkError.hpp"

namespace Trigger
{
namespace SnortSam
{

ExceptionNetworkError::ExceptionNetworkError(const Location    &where,
                                             const std::string &host,
                                             const std::string &details):
  Exception(where, cc("network error during I/O with '", host, "': ", details) )
{
}

} // namespace SnortSam
} // namespace Trigger
