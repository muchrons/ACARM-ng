/*
 * Heartbeats.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_HEARTBEATS_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_HEARTBEATS_HPP_FILE

#include "Logger/Node.hpp"
#include "Persistency/IO/Heartbeats.hpp"
#include "Persistency/IO/Postgres/DBHandle.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief heartbeats implementation on postgres data base.
 */
class Heartbeats: public Persistency::IO::Heartbeats
{
public:
  /** \brief create heartbeats generator.
   *  \param owner    system that will report heartbeats (i.e. ACARM-ng element).
   *  \param t        active transaction.
   *  \param dbHandle handle to data base.
   */
  Heartbeats(const Owner &owner, Transaction &t, DBHandlePtrNN dbHandle);

private:
  virtual void reportImpl(Transaction &t, const Owner &o, const Module &m, Timestamp reported, unsigned int timeout);

  Logger::Node  log_;
  DBHandlePtrNN dbHandle_;
}; // class Heartbeats

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
