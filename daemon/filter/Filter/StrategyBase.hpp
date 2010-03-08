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

  /** \brief gets filter name.
   *  \return name of implemented filter.
   */
  const std::string &getFilterName(void) const
  {
    return name_;
  }

protected:
  /** \brief create instance.
   */
  explicit StrategyBase(const std::string &name);

  /** \brief call allows interruption of call sequence.
   *
   *  this method should be called in long-running algorithms, so that
   *  aborting thread will be reasonable fast.
   */
  void interruptionPoint(void) const;

  const Logger::Node               log_;    ///< logger node to log messages to
  Persistency::IO::ConnectionPtrNN conn_;   ///< connection to persistency

private:
  const std::string                name_;
}; // class StrategyBase

} // namespace Filter

#endif
