/*
 * NodeConfPtr.hpp
 *
 */
#ifndef INCLUDE_LOGGER_NODECONFPTR_HPP_FILE
#define INCLUDE_LOGGER_NODECONFPTR_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>

namespace Logger
{

// forward declaration for NodeConf
class NodeConf;

/** \brief shared pointer to configuration.
 */
typedef boost::shared_ptr<NodeConf> NodeConfPtr;

} // namespace Logger

#endif
