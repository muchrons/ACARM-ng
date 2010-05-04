/*
 * LogicImpl.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_LOGICIMPL_HPP_FILE
#define INCLUDE_PREPROCESSOR_LOGICIMPL_HPP_FILE

#include <vector>

#include "ConfigIO/Preprocessor/Config.hpp"
#include "Persistency/Alert.hpp"
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
  LogicImpl(const ConfigIO::Preprocessor::Config &cfg);

  /** \brief checks if given alert is to be accepted according to defined rules.
   *  \param alert alert to be checked.
   *  \return true, if alert is to be accepted, false otherwise.
   */
  bool checkAccept(const Persistency::Alert &alert) const;

private:
  typedef std::vector<Section> SectionSet;
  SectionSet ss_;
}; // class LogicImpl

} // namespace Preprocessor

#endif
