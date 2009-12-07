/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_CONNECTION_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "Persistency/IO/Alert.hpp"

namespace Persistency
{
namespace IO
{

/** \brief Connection to the persistency API
 */
class Connection: private boost::noncopyable
{
public:
  virtual ~Connection(void);
private:
  boost::scoped_ptr<IO::Alert> alertIOptr_;
}; // class Connection


/** \brief smart pointer to this type.
 */
typedef boost::shared_ptr<Connection> ConnectionPtr;

} // namespace IO
} // namespace Persistency

#endif
