/*
 * MetaAlert.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_METAALERT_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_METAALERT_HPP_FILE

/* public header */

#include <string>

#include "Logger/Node.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "PythonAPI/Persistency/OptionalString.hpp"
#include "PythonAPI/Exception.hpp"


namespace PythonAPI
{
namespace Persistency
{

/** \brief string access facade for Persistnecy::MetaAlert object.
 *
 *  this is wrapper code to be exported to python.
 */
class MetaAlert
{
public:
  /** \brief create facade object for a given node.
   *  \param gn graph node to be accessed via string facade.
   */
  explicit MetaAlert(::Persistency::ConstGraphNodePtrNN gn);

  /** \brief gets value of a requested field.
   *  \param path element to access (ex. "metaalert.alert.name").
   *  \return value of a parameter in the path. on error returns NULL.
   */
  OptionalString get(const std::string &path) const;

private:
  Logger::Node                       log_;
  ::Persistency::ConstGraphNodePtrNN node_;
}; // class StrAccess

} // namespace Persistency
} // namespace PythonAPI

#endif
