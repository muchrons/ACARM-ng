/*
 * ExceptionCryptoFailed.cpp
 *
 */
#include "Trigger/SnortSam/ExceptionCryptoFailed.hpp"

namespace Trigger
{
namespace SnortSam
{

ExceptionCryptoFailed::ExceptionCryptoFailed(const Location    &where,
                                             const std::string &details):
  Exception(where, cc("cryptographic routine failed: ", details) )
{
}

} // namespace SnortSam
} // namespace Trigger
