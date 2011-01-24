/*
 * Builder.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_PLUGINS_BUILDER_HPP_FILE
#define INCLUDE_SYSTEM_PLUGINS_BUILDER_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "System/Plugins/DynamicObject.hpp"
#include "System/Plugins/ExceptionCannotOpenSharedObject.hpp"

namespace System
{
namespace Plugins
{

/** \brief builder of handles.
 */
class Builder
{
public:
  /** \brief create dynamic objects builder.
   *  \param makeSymbolsVisible true if exported symbols are to be visible globally, false otherwise.
   *  \param lazyResolving      true if symbols are to be resolved lazy, false otherwise.
   */
  explicit Builder(bool makeSymbolsVisible=false, bool lazyResolving=true);

  /** \brief opens given shared object and returns dynamic object (with handle) to it.
   *  \param so shared object path.
   *  \return dynamic object with valid handle.
   */
  DynamicObject open(const boost::filesystem::path &so) const;

private:
  int flags_;
}; // class Builder

} // namespace Plugins
} // namespace System

#endif
