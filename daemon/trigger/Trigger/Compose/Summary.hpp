/*
 * Summary.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_COMPOSE_SUMMARY_HPP_FILE
#define INCLUDE_TRIGGER_COMPOSE_SUMMARY_HPP_FILE

/* public header */

#include <sstream>
#include <boost/noncopyable.hpp>

#include "Persistency/GraphNodePtr.hpp"

namespace Trigger
{
namespace Compose
{

/** \brief creates basic output (summary) report.
 */
class Summary: private boost::noncopyable
{
public:
  /** \brief append report to stream.
   *  \param ss   stream to append report to.
   *  \param node node to be reported.
   */
  static void append(std::stringstream &ss, const Persistency::GraphNodePtrNN &node);
}; // class Summary

} // namespace Compose
} // namespace Trigger

#endif
