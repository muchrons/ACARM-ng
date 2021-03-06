/*
 * LogicImpl.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_LOGICIMPL_HPP_FILE
#define INCLUDE_PREPROCESSOR_LOGICIMPL_HPP_FILE

#include <vector>

#include "ConfigIO/Preprocessor/Config.hpp"
#include "Logger/Node.hpp"
#include "Persistency/GraphNode.hpp"
#include "Preprocessor/Section.hpp"

namespace Preprocessor
{

/** \brief main preprocessor's logic - the implementation.
 */
class LogicImpl
{
public:
  /** \brief create object instance from given configuration.
   */
  explicit LogicImpl(const ConfigIO::Preprocessor::Config &cfg);

  /** \brief checks if given alert is to be accepted according to defined rules.
   *  \param node (meta-)alert to be checked.
   *  \return true, if alert is to be accepted, false otherwise.
   */
  bool checkAccept(const Persistency::ConstGraphNodePtrNN &node) const;

private:
  typedef std::vector<Section> SectionSet;

  Logger::Node log_;
  SectionSet   ss_;
}; // class LogicImpl

} // namespace Preprocessor

#endif
