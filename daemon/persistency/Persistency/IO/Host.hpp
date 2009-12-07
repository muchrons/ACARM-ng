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
  Host(Persistency::HostPtr  host,
       const Transaction    &t);

  virtual ~Host(void);

  virtual void setName(const Persistency::Host::Name &name) = 0;
  // TODO

protected:
  Persistency::Host &get(void);

private:
  Persistency::HostPtr  host_;
  const Transaction    &t_;
}; // class Host


typedef std::auto_ptr<Host> HostAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
