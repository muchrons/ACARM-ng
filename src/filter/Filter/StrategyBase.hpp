/*
 * StrategyBase.hpp
 *
 */
#ifndef INCLUDE_FILTER_STRATEGYBASE_HPP_FILE
#define INCLUDE_FILTER_STRATEGYBASE_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Persistency/GraphNode.hpp"
#include "Core/Types/Proc/TypeName.hpp"
#include "Core/Types/Proc/InstanceName.hpp"
// this header is not used directly, but it is required for the derived
// classes to implement special, static method.
#include "Core/Types/Proc/EntryControlList.hpp"

namespace Filter
{

/** \brief common parts for Strategy<> template.
 */
class StrategyBase: private boost::noncopyable
{
public:
  /** \brief deallocate object.
   */
  virtual ~StrategyBase(void);

  /** \brief gets filter type.
   *  \return type of implemented filter.
   */
  const Core::Types::Proc::TypeName &getFilterType(void) const
  {
    return type_;
  }
  /** \brief gets filter name.
   *  \return name of this filter.
   */
  const Core::Types::Proc::InstanceName &getFilterName(void) const
  {
    return name_;
  }

protected:
  /** \brief create instance.
   */
  explicit StrategyBase(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name);

  /** \brief call allows interruption of call sequence.
   *
   *  this method should be called in long-running algorithms, so that
   *  aborting thread will be reasonable fast.
   */
  void interruptionPoint(void) const;

  const Logger::Node               log_;    ///< logger node to log messages to
  Persistency::IO::ConnectionPtrNN conn_;   ///< connection to persistency

private:
  const Core::Types::Proc::TypeName     type_;
  const Core::Types::Proc::InstanceName name_;
}; // class StrategyBase

} // namespace Filter

#endif
