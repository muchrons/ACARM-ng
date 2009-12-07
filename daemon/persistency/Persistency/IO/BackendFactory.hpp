/*
 * BackendFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_BACKENDFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_BACKENDFACTORY_HPP_FILE

/* public header */

#include "Commons/Factory/AbstractFactorySingleton.hpp"

#include "Persistency/IO/Connection.hpp"


namespace Persistency
{
namespace IO
{

/** \brief abstract factory producing different persistency backends on demand.
 */
typedef Commons::Factory::AbstractFactorySingleton<Connection> BackendFactory;

} // namespace IO
} // namespace Persistency

#endif
