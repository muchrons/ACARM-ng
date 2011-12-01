/*
 * Heartbeats.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_HEARTBEATS_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_HEARTBEATS_HPP_FILE

#include "Persistency/IO/Heartbeats.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief heartbeats stub.
 */
class Heartbeats: public Persistency::IO::Heartbeats
{
public:
  /** \brief create heartbeats generator.
   *  \param owner system that will report heartbeats (i.e. ACARM-ng element).
   *  \param t     active transaction.
   *  \param h     persistency handle.
   */
  Heartbeats(const Owner &owner, Transaction &t, int h);

  size_t reportCalls_;  ///< counter incremented on each call to implementation.

private:
  virtual void reportImpl(Transaction &t, const Owner &o, const Module &m, Timestamp reported, unsigned int timeout);
}; // class Heartbeats

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
