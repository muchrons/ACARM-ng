/*
 * Host.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_HOST_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_HOST_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

#include "Persistency/Host.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::Host API
 */
class Host: private boost::noncopyable
{
public:
  /** \brief create host persistency proxy object for given host.
   *  \param host host to work on.
   *  \param t    active transaction.
   */
  Host(Persistency::HostPtrNN  host,
       Transaction            &t);
  /** \brief virtual d-tor for polymorphic base class.
   */
  virtual ~Host(void);
  /** \brief sets name of given host.
   *  \param name name to be set.
   */
  void setName(const Persistency::Host::Name &name);

protected:
  /** \brief gets read-only host object access.
   *  \return not null pointer to host object's working on.
   */
  Persistency::HostPtrNN get(void);

private:
  virtual void setNameImpl(Transaction                   &t,
                           const Persistency::Host::Name &name) = 0;

  Persistency::HostPtr  host_;
  Transaction          &t_;
}; // class Host


/** \brief auto pointer to IO::Host. */
typedef std::auto_ptr<Host> HostAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
