/*
 * Restorer.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_RESTORER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_RESTORER_HPP_FILE

#include "Persistency/IO/Restorer.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

/** \brief restore oprations on peristency API
 */
class Restorer: public IO::Restorer
{
public:
  /** \brief create stub.
   *  \param t       active transaction.
   *  \param handler persistency handler.
   */
  Restorer(Transaction &t,
           int          handler);

  /** number of times 'restoreAllInUseImpl()' has been called. */
  size_t restoreAllCalls_;
  /** number of times 'restoreBetweenImpl()' has been called. */
  size_t restoreBetweenCalls_;

private:
  virtual void restoreAllInUseImpl(Transaction &t, NodesVector &out);
  virtual void restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to);
}; // class Alert

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
