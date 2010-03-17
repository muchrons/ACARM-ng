/*
 * Processors.hpp
 *
 */
#ifndef INCLUDE_CORE_PROCESSORS_HPP_FILE
#define INCLUDE_CORE_PROCESSORS_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Persistency/GraphNode.hpp"
#include "Core/Types/NodesFifo.hpp"
#include "Core/Types/Proc/ProcessorsCollection.hpp"

namespace Core
{

/** \brief class representing processors of nodes.
 */
class Processors: private boost::noncopyable
{
public:
  /** \brief create all processors and runs them.
   *  \param queue queue to process (i.e. to work on).
   */
  explicit Processors(Core::Types::NodesFifo &queue);
  /** \brief clean-up data.
   */
  ~Processors(void);

  /** \brief call that performs single step in processing queue.
   *
   *  call reads first element from queue and passes it to all the processors
   *  and then exits. notice that there might be more elements in queue,
   *  therefore this call should be put inside loop.
   */
  void process(void);

private:
  void append(const Core::Types::Proc::ProcessorsCollection &newProcs);

  Logger::Node                             log_;
  Core::Types::NodesFifo                  &queue_;
  Core::Types::Proc::ProcessorsCollection  procs_;
}; // class Source

} // namespace Core

#endif
