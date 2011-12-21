/*
 * Logic.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_LOGIC_HPP_FILE
#define INCLUDE_PREPROCESSOR_LOGIC_HPP_FILE

/* public header */

#include <boost/scoped_ptr.hpp>

#include "ConfigIO/Preprocessor/Config.hpp"
#include "Persistency/GraphNode.hpp"
#include "Preprocessor/Exception.hpp"

namespace Preprocessor
{

// forward declaration of implementation
class LogicImpl;

/** \brief main preprocessor's logic - interface.
 */
class Logic
{
public:
  /** \brief create object instance with explicitly specified configuration.
   *  \param cfg configuration to be used for creating preprocessor's logic.
   */
  explicit Logic(const ConfigIO::Preprocessor::Config &cfg);
  /** \brief ensure proper destruction of object.
   */
  ~Logic(void);

  /** \brief checks if given alert is to be accepted according to defined rules.
   *  \param node (meta-)alert to be checked.
   *  \return true, if alert is to be accepted, false otherwise.
   */
  bool checkAccept(const Persistency::ConstGraphNodePtrNN &node) const;

private:
  boost::scoped_ptr<LogicImpl> pimpl_;
}; // class Logic

} // namespace Preprocessor

#endif
