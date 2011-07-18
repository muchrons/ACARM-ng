/*
 * ModulesInitList.cpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_MODULESINITLIST_HPP_FILE
#define INCLUDE_PYTHONAPI_MODULESINITLIST_HPP_FILE

#include <vector>
#include <boost/tuple/tuple.hpp>

#include "PythonAPI/ModuleInitFunction.hpp"

namespace PythonAPI
{

/** \brief colleciton holding user-provided python modules to be initialized.
 */
class ModulesInitList
{
private:
  typedef boost::tuple<const char*, ModuleInitFunction> ModuleInitSpec;
  typedef std::vector<ModuleInitSpec>                   ImportedModulesList;

public:
  typedef ImportedModulesList::const_iterator const_iterator;

  /** \brief static call to schedule given module for importing.
   *  \param module module name (must be a compile-time string constant).
   *  \param init   init funtion (as defined by boost::python's macros).
   */
  void scheduleImport(const char *module, ModuleInitFunction init)
  {
    mods_.push_back( ModuleInitSpec(module, init) );
  }

  const_iterator begin(void) const
  {
    return mods_.begin();
  }
  const_iterator end(void) const
  {
    return mods_.end();
  }

  /** \brief gets the number of schedules modules.
   */
  size_t count(void)
  {
    return mods_.size();
  }

  void clear(void)
  {
    ImportedModulesList tmp;
    mods_.swap(tmp);
  }

private:
  ImportedModulesList mods_;
}; // class ModulesInitList

} // namespace PythonAPI

#endif
