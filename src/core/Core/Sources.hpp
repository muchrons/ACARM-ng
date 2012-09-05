/*
 * Sources.hpp
 *
 */
#ifndef INCLUDE_CORE_SOURCES_HPP_FILE
#define INCLUDE_CORE_SOURCES_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Preprocessor/Logic.hpp"
#include "Core/Types/AlertsFifo.hpp"
#include "Input/Factory.hpp"
#include "Core/MemoryUseChecker.hpp"

namespace Core
{

/** \brief common entry point gathering all inputs together.
 */
class Sources: private boost::noncopyable
{
public:
  /** \brief create all inputs and start reading from them.
   * \param memCheck source limiter object
   */
  Sources(const MemoryUseChecker* memCheck);
  /** \brief clean-up data.
   */
  ~Sources(void);
  /** \brief read next entry from input(s).
   *  \return newly read leaf.
   */
  Persistency::GraphNodePtrNN read(void);

private:
  Logger::Node                     log_;
  Persistency::IO::ConnectionPtrNN conn_;
  Core::Types::AlertsFifo          queue_;
  Input::InputsCollection          inputs_;
  Preprocessor::Logic              preproc_;
  const MemoryUseChecker*          memCheck_;
}; // class Sources

} // namespace Core

#endif
