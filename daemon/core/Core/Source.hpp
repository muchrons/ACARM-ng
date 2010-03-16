/*
 * Source.hpp
 *
 */
#ifndef INCLUDE_CORE_SOURCE_HPP_FILE
#define INCLUDE_CORE_SOURCE_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Core/Types/AlertsFifo.hpp"
#include "Input/Factory.hpp"

namespace Core
{

/** \brief common entry point gathering all inputs together.
 */
class Source: private boost::noncopyable
{
public:
  /** \brief create all inputs and start reading from them.
   */
  Source(void);
  /** \brief read next entry from input(s).
   *  \return newly read leaf.
   */
  Persistency::GraphNodePtrNN read(void);

private:
  Persistency::AlertPtrNN readAlert(void);

  Persistency::IO::ConnectionPtrNN conn_;
  Core::Types::AlertsFifo          queue_;
  Input::InputsCollection          inputs_;
}; // class Source

} // namespace Core

#endif
