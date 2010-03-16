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

class Source: private boost::noncopyable
{
public:
  Source(void);

  Persistency::GraphNodePtrNN read(void);

private:
  Persistency::AlertPtrNN readAlert(void);

  Persistency::IO::ConnectionPtrNN conn_;
  Core::Types::AlertsFifo          queue_;
  Input::InputsCollection          inputs_;
}; // class Source

} // namespace Core

#endif
