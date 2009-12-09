/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_CONNECTION_HPP_FILE

#include "Persistency/IO/ConnectionHelper.hpp"
#include "Persistency/IO/Stubs/TransactionAPI.hpp"
#include "Persistency/IO/Stubs/Alert.hpp"
#include "Persistency/IO/Stubs/Host.hpp"
#include "Persistency/IO/Stubs/MetaAlert.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{
namespace detail
{
/** \brief helper typedef to make names shorter.
 */
typedef IO::ConnectionHelper<int,   // connection handler - anything...
                             Stubs::TransactionAPI,
                             Stubs::Alert,
                             Stubs::Host,
                             Stubs::MetaAlert> ConnectionBase;
} // namespace detail

/** \brief stub of connection element
 */
class Connection: public detail::ConnectionBase
{
public:
  /** \brief create new connection to nowhere.
   */
  Connection(void);
}; // class Connection

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
