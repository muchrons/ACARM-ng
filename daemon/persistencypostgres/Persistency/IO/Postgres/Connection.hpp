/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE

#include "Persistency/IO/ConnectionHelper.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"
#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief implementation of Connection for PostgreSQL. */
typedef IO::ConnectionHelper<DBHandlerPtrNN,
                             TransactionAPI,
                             Alert,
                             Host,
                             MetaAlert,
                             Restorer>   Connection;

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
